/*
 *  Copyright 2012 David Cameron
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

#ifndef GFAL_XROOTD_PLUGIN_INTERFACE_H_
#define GFAL_XROOTD_PLUGIN_INTERFACE_H_

#include <gfal_plugins_api.h>

#define XROOTD_CONFIG_GROUP "XROOTD PLUGIN"

extern "C" {

extern GQuark xrootd_domain;


int gfal_xrootd_statG(plugin_handle handle, const char* name, struct stat* buff, GError ** err);

gfal_file_handle gfal_xrootd_openG(plugin_handle handle, const char *path, int flag, mode_t mode, GError ** err);

ssize_t gfal_xrootd_readG(plugin_handle handle, gfal_file_handle fd, void *buff, size_t count, GError ** err);

ssize_t gfal_xrootd_writeG(plugin_handle handle, gfal_file_handle fd, const void *buff, size_t count, GError ** err);

off_t gfal_xrootd_lseekG(plugin_handle handle, gfal_file_handle fd, off_t offset, int whence, GError **err);

int gfal_xrootd_closeG(plugin_handle handle, gfal_file_handle fd, GError ** err);

int gfal_xrootd_mkdirpG(plugin_handle plugin_data, const char *url, mode_t mode, gboolean pflag, GError **err);

int gfal_xrootd_chmodG(plugin_handle plugin_data, const char *url, mode_t mode, GError **err);

int gfal_xrootd_unlinkG(plugin_handle plugin_data, const char *url, GError **err);

int gfal_xrootd_rmdirG(plugin_handle plugin_data, const char *url, GError **err);

int gfal_xrootd_accessG(plugin_handle plugin_data, const char *url, int mode, GError **err);

int gfal_xrootd_renameG(plugin_handle plugin_data, const char *oldurl, const char *urlnew, GError **err);

gfal_file_handle gfal_xrootd_opendirG(plugin_handle plugin_data, const char* url, GError** err);

struct dirent* gfal_xrootd_readdirG(plugin_handle plugin_data, gfal_file_handle dir_desc, GError** err);

int gfal_xrootd_closedirG(plugin_handle plugin_data, gfal_file_handle dir_desc, GError** err);

int gfal_xrootd_checksumG(plugin_handle data, const char* url, const char* check_type,
                          char * checksum_buffer, size_t buffer_length,
                          off_t start_offset, size_t data_length,
                          GError ** err);

int gfal_xrootd_3rdcopy_check(plugin_handle plugin_data,
                              const char* src, const char* dst,
                              gfal_url2_check check);

int gfal_xrootd_3rd_copy(plugin_handle plugin_data, gfal2_context_t context,
                         gfalt_params_t params,
                         const char* src, const char* dst,
                         GError** err);

const char* gfal_xrootd_getName();

void set_xrootd_log_level();

}

#endif /* GFAL_XROOTD_PLUGIN_INTERFACE_H_ */
