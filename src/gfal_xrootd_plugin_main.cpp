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

#include <string.h>
#include <sys/types.h>

#include <gfal_plugins_api.h>
#include "gfal_xrootd_plugin_interface.h"

extern "C" {

gboolean gfal_xrootd_check_url(plugin_handle ch, const char* url,  plugin_mode mode, GError** err);

gfal_plugin_interface gfal_plugin_init(gfal_handle handle, GError** err) {

  gfal_plugin_interface xrootd_plugin;
  memset(&xrootd_plugin,0,sizeof(gfal_plugin_interface)); // clear the plugin

  // set xrootd log level
  set_xrootd_log_level();

  xrootd_plugin.getName = &gfal_xrootd_getName;
  xrootd_plugin.check_plugin_url = &gfal_xrootd_check_url;

  xrootd_plugin.openG = &gfal_xrootd_openG;
  xrootd_plugin.closeG = &gfal_xrootd_closeG;
  xrootd_plugin.readG = &gfal_xrootd_readG;
  xrootd_plugin.writeG = &gfal_xrootd_writeG;
  xrootd_plugin.lseekG = &gfal_xrootd_lseekG;

  xrootd_plugin.statG = &gfal_xrootd_statG;
  xrootd_plugin.lstatG = &gfal_xrootd_statG;

  xrootd_plugin.preadG = NULL; // &gfal_xrootd_preadG;
  xrootd_plugin.pwriteG = NULL; // &gfal_xrootd_pwriteG;

  xrootd_plugin.mkdirpG = &gfal_xrootd_mkdirpG;
  xrootd_plugin.chmodG = &gfal_xrootd_chmodG;
  xrootd_plugin.unlinkG = &gfal_xrootd_unlinkG;
  xrootd_plugin.rmdirG = &gfal_xrootd_rmdirG;
  xrootd_plugin.accessG = &gfal_xrootd_accessG;
  xrootd_plugin.renameG = &gfal_xrootd_renameG;

  xrootd_plugin.opendirG = &gfal_xrootd_opendirG;
  xrootd_plugin.readdirG = &gfal_xrootd_readdirG;
  xrootd_plugin.closedirG = &gfal_xrootd_closedirG;

  xrootd_plugin.getxattrG = NULL; // &gfal_xrootd_getxattrG;
  xrootd_plugin.listxattrG = NULL; // &gfal_xrootd_listxattrG;
  xrootd_plugin.setxattrG = NULL; // &gfal_xrootd_setxattrG;

  xrootd_plugin.readlinkG = NULL; // symlinks not supported on xrootd
  xrootd_plugin.symlinkG = NULL; // symlinks not supported on xrootd

  xrootd_plugin.plugin_delete = NULL; // &gfal_xrootd_destroyG;

  return xrootd_plugin;
}

gboolean gfal_xrootd_check_url(plugin_handle ch, const char* url, plugin_mode mode, GError** err) {

  if (strncmp(url, "root://", 7) != 0) return FALSE;

  int ret;
  switch(mode) {
      case GFAL_PLUGIN_STAT:
      case GFAL_PLUGIN_LSTAT:
      case GFAL_PLUGIN_OPEN:
      case GFAL_PLUGIN_MKDIR:
      case GFAL_PLUGIN_CHMOD:
      case GFAL_PLUGIN_UNLINK:
      case GFAL_PLUGIN_RMDIR:
      case GFAL_PLUGIN_ACCESS:
      case GFAL_PLUGIN_RENAME:
      case GFAL_PLUGIN_OPENDIR:
        ret = TRUE;
        break;
      default:
        ret = FALSE;
        break;
  }
  return ret;
}

} // extern "C"
