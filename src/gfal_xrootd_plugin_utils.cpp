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

#include <fcntl.h>

#include "gfal_xrootd_plugin_utils.h"

void file_mode_to_xrootd_ints(mode_t mode, int& user, int& group, int& other) {
  user = 0;
  group = 0;
  other = 0;

  if (mode & S_IRUSR) user += 4;
  if (mode & S_IWUSR) user += 2;
  if (mode & S_IXUSR) user += 1;

  if (mode & S_IRGRP) group += 4;
  if (mode & S_IWGRP) group += 2;
  if (mode & S_IXGRP) group += 1;

  if (mode & S_IROTH) other += 4;
  if (mode & S_IWOTH) other += 2;
  if (mode & S_IXOTH) other += 1;
}

void reset_stat(struct stat& st) {
  timespec t;
  t.tv_sec = 0;
  t.tv_nsec = 0;
  st.st_mode = 0;
  st.st_atim = t;
  st.st_ctim = t;
  st.st_mtim = t;
  st.st_blksize = 0;
  st.st_blocks = 0;
  st.st_dev = 0;
  st.st_gid = 0;
  st.st_ino = 0;
  st.st_nlink = 0;
  st.st_rdev = 0;
  st.st_size = 0;
  st.st_uid = 0;
}

std::string sanitize_url(const char* url) {
  const char* p = url + 7; // Jump over root://
  p = strchr(p, '/');

  std::string sanitized;
  if (p == NULL) {
    sanitized = std::string(url) + "///";
  }
  else if (strncmp(p, "///", 3) == 0) {
    sanitized = url;
  }
  else if (strncmp(p, "//", 2) == 0) {
    sanitized = std::string(url, p - url) + "/" + p;
  }
  else {
    sanitized = std::string(url, p - url) + "//" + p;
  }

  return sanitized;
}
