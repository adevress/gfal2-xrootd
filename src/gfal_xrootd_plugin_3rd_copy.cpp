/*
 *  Copyright 2013 Alejandro Álvarez Ayllón
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <gfal_plugins_api.h>
#include "gfal_xrootd_plugin_interface.h"

#undef TRUE
#undef FALSE

#include <XrdCl/XrdClThirdPartyCopyJob.hh>



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
  XrdCl::ThirdPartyCopyJob thirdPartyCopy(src, dst);
  XrdCl::XRootDStatus status = thirdPartyCopy.Run(NULL);

  if (status.IsOK()) {
    return 0;
  }
  else {
    g_set_error(err, 0, status.errNo,
                "[%s] Error on XrdCl::ThirdPartyCopyJob: %s",
                __func__, status.GetErrorMessage().c_str());
    return -1;
  }
}
