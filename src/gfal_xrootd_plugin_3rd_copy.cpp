/*
* Copyright @ Members of the EMI Collaboration, 2010.
* See www.eu-emi.eu for details on the copyright holders.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
* 
* Authors : David Cameron, Alejandro Alvarez Ayllon, Adrien Devresse
* 
*/


#include <ctype.h>
#include <gfal_plugins_api.h>
#include <transfer/gfal_transfer.h>
#include <transfer/gfal_transfer_plugins.h>
#include <transfer/gfal_transfer_types.h>
#include "gfal_xrootd_plugin_interface.h"
#include "gfal_xrootd_plugin_utils.h"

#undef TRUE
#undef FALSE

#include <XrdCl/XrdClCopyProcess.hh>

#define XROOTD_DEFAULT_CHECKSUM "COPY_CHECKSUM_TYPE"



class CopyFeedback: public XrdCl::CopyProgressHandler {
public:
  CopyFeedback(gfalt_params_t p): params(p), startTime(0)
  {
    this->monitorCallback = gfalt_get_monitor_callback(this->params, NULL);
    monitorCallbackData = gfalt_transfer_status_create(&this->status);
  }

  virtual ~CopyFeedback()
  {
    gfalt_transfer_status_delete(this->monitorCallbackData);
  }


  void BeginJob(uint16_t jobNum, uint16_t jobTotal,
                const XrdCl::URL *source, const XrdCl::URL *destination)
  {
    this->startTime   = time(NULL);
    this->source      = source->GetURL();
    this->destination = destination->GetURL();

    plugin_trigger_event(this->params, xrootd_domain,
                         GFAL_EVENT_NONE, GFAL_EVENT_TRANSFER_ENTER,
                         "%s => %s",
                         this->source.c_str(), this->destination.c_str());
  }


  void EndJob(const XrdCl::XRootDStatus &status)
  {
    plugin_trigger_event(this->params, xrootd_domain,
                         GFAL_EVENT_NONE, GFAL_EVENT_TRANSFER_EXIT,
                         "%s", status.ToStr().c_str());
  }


  void JobProgress(uint64_t bytesProcessed, uint64_t bytesTotal)
  {
    if (this->monitorCallback) {
      time_t now = time(NULL);
      time_t elapsed = now - this->startTime;

      this->status.status           = 0;
      this->status.bytes_transfered = bytesProcessed;
      this->status.transfer_time    = elapsed;
      if (elapsed > 0)
        this->status.average_baudrate = bytesProcessed / elapsed;
      this->status.instant_baudrate   = this->status.average_baudrate;

      this->monitorCallback(this->monitorCallbackData,
                            this->source.c_str(), this->destination.c_str(),
                            NULL);
    }
  }


private:
  gfalt_params_t     params;
  gfalt_monitor_func monitorCallback;

  gfalt_transfer_status_t monitorCallbackData;
  gfalt_hook_transfer_plugin_t status;
  time_t startTime;

  std::string source, destination;
};



int gfal_xrootd_3rdcopy_check(plugin_handle plugin_data,
                              const char* src, const char* dst,
                              gfal_url2_check check)
{
  if (check != GFAL_FILE_COPY) return 0;

  return (strncmp(src, "root://", 7) == 0 &&
          strncmp(dst, "root://", 7) == 0);
}



int gfal_xrootd_3rd_copy(plugin_handle plugin_data, gfal2_context_t context,
                         gfalt_params_t params,
                         const char* src, const char* dst,
                         GError** err)
{
  GError* internalError = NULL;

  XrdCl::JobDescriptor job;

  job.source.FromString(sanitize_url(src));
  job.target.FromString(sanitize_url(dst));
  job.force              = gfalt_get_replace_existing_file(params, NULL);;
  job.posc               = true;
  job.thirdParty         = true;
  job.thirdPartyFallBack = false;
  job.checkSumPrint      = false;

  char checksumType[64];
  char checksumValue[512];
  gfalt_get_user_defined_checksum(params,
                                  checksumType, sizeof(checksumType),
                                  checksumValue, sizeof(checksumValue),
                                  NULL);
  if (!checksumType[0] || !checksumValue[0]) {
    char* defaultChecksumType = gfal2_get_opt_string(context,
                                                     XROOTD_CONFIG_GROUP, XROOTD_DEFAULT_CHECKSUM,
                                                     &internalError);
    if (internalError) {
      g_set_error(err, xrootd_domain,
                  internalError->code,
                  "[%s] %s", __func__, internalError->message);
      g_error_free(internalError);
      return -1;
    }

    strncpy(checksumType, defaultChecksumType, sizeof(checksumType));
    g_free(defaultChecksumType);
  }

  job.checkSumType   = predefinedChecksumTypeToLower(checksumType);
  job.checkSumPreset = checksumValue;

  XrdCl::CopyProcess process;
  process.AddJob(&job);


  XrdCl::XRootDStatus status = process.Prepare();
  if (!status.IsOK()) {
    g_set_error(err, 0, status.errNo,
                "[%s] Error on XrdCl::CopyProcess::Prepare(): %s",
                __func__, status.ToStr().c_str());
    return -1;
  }


  CopyFeedback feedback(params);
  status = process.Run(&feedback);

  if (!status.IsOK()) {
    g_set_error(err, 0, status.errNo,
                "[%s] Error on XrdCl::CopyProcess::Run(): %s",
                __func__, status.ToStr().c_str());
    return -1;
  }

  return 0;
}
