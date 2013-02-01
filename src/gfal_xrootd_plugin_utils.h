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

#ifndef GFAL_XROOTD_PLUGIN_UTILS_H_
#define GFAL_XROOTD_PLUGIN_UTILS_H_

#include <string>
#include <sys/stat.h>

/// Convert file mode_t to ints
void file_mode_to_xrootd_ints(mode_t mode, int& user, int& group, int& other);

/// Initialise all stat fields to zero
void reset_stat(struct stat& st);

/// Return the same URL, but making sure the path is always relative
std::string sanitize_url(const char* url);

/// If the checksum type is one of the predefined ones, always lowercase
/// @note adler32, crc32, md5
std::string predefinedChecksumTypeToLower(const std::string& type);

#endif /* GFAL_XROOTD_PLUGIN_UTILS_H_ */
