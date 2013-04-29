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



#include <iostream>
#include <sys/stat.h>

// This header provides all the required functions except chmod
#include <XrdPosix/XrdPosixXrootd.hh>

// This header is required for chmod
#include <XrdClient/XrdClientAdmin.hh>

// TRUE and FALSE are defined in Glib and xrootd headers
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

#include <gfal_plugins_api.h>
#include "gfal_xrootd_plugin_interface.h"
#include "gfal_xrootd_plugin_utils.h"


GQuark xrootd_domain = g_quark_from_static_string("xroot");


void set_xrootd_log_level() {
  // Note: xrootd lib logs to stderr
  if (gfal_get_verbose() & GFAL_VERBOSE_TRACE) XrdPosixXrootd::setDebug(3);
  else if (gfal_get_verbose() & GFAL_VERBOSE_DEBUG) XrdPosixXrootd::setDebug(2);
  else if (gfal_get_verbose() & GFAL_VERBOSE_VERBOSE) XrdPosixXrootd::setDebug(1);
  else XrdPosixXrootd::setDebug(0);
}

int gfal_xrootd_statG(plugin_handle handle, const char* path, struct stat* buff, GError ** err) {

  std::string sanitizedUrl = sanitize_url(path);

  // reset stat fields
  reset_stat(*buff);

  if (XrdPosixXrootd::Stat(sanitizedUrl.c_str(), buff) != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to stat file", __func__);
    return -1;
  }
  return 0;
}

gfal_file_handle gfal_xrootd_openG(plugin_handle handle, const char *path, int flag, mode_t mode, GError ** err) {

  std::string sanitizedUrl = sanitize_url(path);

  int* fd = new int(XrdPosixXrootd::Open(sanitizedUrl.c_str(), flag, mode));
  if (*fd == -1) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to open file", __func__);
    delete fd;
    return NULL;
  }
  return gfal_file_handle_new(gfal_xrootd_getName(), (gpointer) fd);
}

ssize_t gfal_xrootd_readG(plugin_handle handle, gfal_file_handle fd, void *buff, size_t count, GError ** err) {

  int * fdesc = (int*)(gfal_file_handle_get_fdesc(fd));
  if (!fdesc) {
    g_set_error(err, xrootd_domain, errno, "[%s] Bad file handle", __func__);
    return -1;
  }
  ssize_t l = XrdPosixXrootd::Read(*fdesc, buff, count);
  if (l < 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed while reading from file", __func__);
    return -1;
  }
  return l;
}

ssize_t gfal_xrootd_writeG(plugin_handle handle, gfal_file_handle fd, const void *buff, size_t count, GError ** err) {

  int * fdesc = (int*)(gfal_file_handle_get_fdesc(fd));
  if (!fdesc) {
    g_set_error(err, xrootd_domain, errno, "[%s] Bad file handle", __func__);
    return -1;
  }
  ssize_t l = XrdPosixXrootd::Write(*fdesc, buff, count);
  if (l < 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed while writing to file", __func__);
    return -1;
  }
  return l;
}


off_t gfal_xrootd_lseekG(plugin_handle handle, gfal_file_handle fd, off_t offset, int whence, GError **err) {

  int * fdesc = (int*)(gfal_file_handle_get_fdesc(fd));
  if (!fdesc) {
    g_set_error(err, xrootd_domain, errno, "[%s] Bad file handle", __func__);
    return -1;
  }
  off_t l = XrdPosixXrootd::Lseek(*fdesc, offset, whence);
  if (l < 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to seek within file", __func__);
    return -1;
  }
  return l;
}

int gfal_xrootd_closeG(plugin_handle handle, gfal_file_handle fd, GError ** err) {

  int r = 0;
  int * fdesc = (int*)(gfal_file_handle_get_fdesc(fd));
  if (fdesc) {
    r = XrdPosixXrootd::Close(*fdesc);
    if (r != 0)
      g_set_error(err, xrootd_domain, errno, "[%s] Failed to close file", __func__);
    delete (int*)(gfal_file_handle_get_fdesc(fd));
  }
  gfal_file_handle_delete(fd);
  return r;
}

int gfal_xrootd_mkdirpG(plugin_handle plugin_data, const char *url, mode_t mode, gboolean pflag, GError **err) {

  std::string sanitizedUrl = sanitize_url(url);

  if (XrdPosixXrootd::Mkdir(sanitizedUrl.c_str(), mode) != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to create directory", __func__);
    return -1;
  }
  return 0;
}

int gfal_xrootd_chmodG(plugin_handle plugin_data, const char *url, mode_t mode, GError **err) {

  std::string sanitizedUrl = sanitize_url(url);

  XrdClientAdmin client(sanitizedUrl.c_str());
  set_xrootd_log_level();

  if (!client.Connect()) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to connect to server", __func__);
    return -1;
  }

  int user, group, other;
  file_mode_to_xrootd_ints(mode, user, group, other);

  XrdClientUrlInfo xrdurl(sanitizedUrl.c_str());

  if (!client.Chmod(xrdurl.File.c_str(), user, group, other)) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to change permissions", __func__);
    return -1;
  }
  return 0;
}

int gfal_xrootd_unlinkG(plugin_handle plugin_data, const char *url, GError **err) {

  std::string sanitizedUrl = sanitize_url(url);

  if (XrdPosixXrootd::Unlink(sanitizedUrl.c_str()) != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to delete file", __func__);
    return -1;
  }
  return 0;
}

int gfal_xrootd_rmdirG(plugin_handle plugin_data, const char *url, GError **err) {

  std::string sanitizedUrl = sanitize_url(url);

  if (XrdPosixXrootd::Rmdir(sanitizedUrl.c_str()) != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to delete directory", __func__);
    return -1;
  }
  return 0;
}

int gfal_xrootd_accessG(plugin_handle plugin_data, const char *url, int mode, GError **err) {

  std::string sanitizedUrl = sanitize_url(url);

  if (XrdPosixXrootd::Access(sanitizedUrl.c_str(), mode) != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to access file or directory", __func__);
    return -1;
  }
  return 0;
}

int gfal_xrootd_renameG(plugin_handle plugin_data, const char *oldurl, const char *urlnew, GError **err) {

  std::string oldSanitizedUrl = sanitize_url(oldurl);
  std::string newSanitizedUrl = sanitize_url(urlnew);

  if (XrdPosixXrootd::Rename(oldSanitizedUrl.c_str(), newSanitizedUrl.c_str()) != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to rename file or directory", __func__);
    return -1;
  }
  return 0;
}

gfal_file_handle gfal_xrootd_opendirG(plugin_handle plugin_data, const char* url, GError** err) {

  std::string sanitizedUrl = sanitize_url(url);

  DIR* dirp = XrdPosixXrootd::Opendir(sanitizedUrl.c_str());
  if (!dirp) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed to open dir", __func__);
    return NULL;
  }
  return gfal_file_handle_new(gfal_xrootd_getName(), (gpointer) dirp);
}

struct dirent* gfal_xrootd_readdirG(plugin_handle plugin_data, gfal_file_handle dir_desc, GError** err) {

  DIR* dirp = (DIR*)(gfal_file_handle_get_fdesc(dir_desc));
  if (!dirp) {
    g_set_error(err, xrootd_domain, errno, "[%s] Bad dir handle", __func__);
    return NULL;
  }
  dirent* entry = XrdPosixXrootd::Readdir(dirp);
  if (!entry && errno != 0) {
    g_set_error(err, xrootd_domain, errno, "[%s] Failed reading directory", __func__);
    return NULL;
  }
  return entry;
}

int gfal_xrootd_closedirG(plugin_handle plugin_data, gfal_file_handle dir_desc, GError** err) {

  // Free all objects associated with this client
  int res = 0;
  DIR* dirp = (DIR*)(gfal_file_handle_get_fdesc(dir_desc));
  if (dirp) {
    res = XrdPosixXrootd::Closedir(dirp);
    if (res != 0)
      g_set_error(err, xrootd_domain, errno, "[%s] Failed to close direcrory", __func__);
    // CloseDir frees DIR object so no need to call delete
  }
  gfal_file_handle_delete(dir_desc);
  return res;
}

int gfal_xrootd_checksumG(plugin_handle data, const char* url, const char* check_type,
                          char * checksum_buffer, size_t buffer_length,
                          off_t start_offset, size_t data_length,
                          GError ** err) {

  std::string sanitizedUrl = sanitize_url(url);
  std::string lowerChecksumType = predefinedChecksumTypeToLower(check_type);

  if (start_offset != 0 || data_length != 0) {
      g_set_error(err, xrootd_domain, ENOTSUP,
                  "[%s] XROOTD does not support partial checksums",
                  __func__);
      return -1;
  }

  time_t mTime;
  if (XrdPosixXrootd::QueryChksum(sanitizedUrl.c_str(), mTime,
                                  checksum_buffer, buffer_length) < 0) {
    g_set_error(err, xrootd_domain, errno,
                "[%s] Could not get the checksum",
                __func__);
    return -1;
  }

  // Note that the returned value is "type value"
  char* space = ::index(checksum_buffer, ' ');
  if (!space) {
    g_set_error(err, xrootd_domain, errno,
                "[%s] Could not get the checksum (Wrong format)",
                __func__);
    return -1;
  }
  *space = '\0';

  if (strncmp(checksum_buffer, lowerChecksumType.c_str(),
              lowerChecksumType.length()) != 0) {
    g_set_error(err, xrootd_domain, errno,
                "[%s] Got '%s' while expecting '%s'",
                __func__, checksum_buffer, lowerChecksumType.c_str());
    return -1;
  }

  strcpy(checksum_buffer, space + 1);

  return 0;
}


const char* gfal_xrootd_getName() {
  return "xrootd";
}
