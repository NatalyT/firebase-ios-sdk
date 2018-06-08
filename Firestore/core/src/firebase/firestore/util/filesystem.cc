/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Firestore/core/src/firebase/firestore/util/filesystem.h"

#include <sys/stat.h>
#include <unistd.h>

#include <string>

#include "Firestore/core/src/firebase/firestore/util/hard_assert.h"
#include "Firestore/core/src/firebase/firestore/util/string_util.h"

namespace firebase {
namespace firestore {
namespace util {

namespace {

#if defined(_WIN32)

// Converts a UTF-8-encoded filename to the equivalent wide character form,
// suitable for use with Windows path-related functions.
std::wstring Utf8ToWide(const std::string& input) {
  // The result from MultiByteToWideChar is size of the buffer required to
  // hold the null terminated result.
  int output_size =
      ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, nullptr, 0);
  if (output_size <= 1) {
    return L"";
  }

  std::wstring output(output_size, '\0');
  int result = ::MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1, &output[0],
                                     output_size);
  output.resize(result > 0 ? result - 1 : 0);
  return output;
}

#endif  // defined(_WIN32)

}  // namespace

Status Dir::Create(absl::string_view pathname) {
  (void)pathname;
  HARD_FAIL("Unimplemented");
}

bool Dir::Exists(const std::string& pathname) {
#if defined(_WIN32)
  DWORD attrs = GetFileAttributesW(Utf8ToWide(pathname).c_str());
  return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY);

#else
  struct stat buffer;
  if (stat(pathname.c_str(), &buffer)) {
    return false;
  }
  return S_ISDIR(buffer.st_mode);
#endif
}

#if !defined(__APPLE__)
std::string Dir::TempDir() {
  const char* env_tmpdir = getenv("TMPDIR");
  if (env_tmpdir != nullptr) {
    return env_tmpdir;
  }

#if defined(__ANDROID__)
  // The /tmp directory doesn't exist as a fallback; each application is
  // supposed to keep its own temporary files. Previously /data/local/tmp may
  // have been reasonable, but current lore points to this being unreliable for
  // writing at higher API levels or certain phone models because default
  // permissions on this directory no longer permit writing.
  //
  // TODO(wilhuff): Validate on recent Android.
#error "Not yet sure about temporary file locations on Android."
  return "/data/local/tmp";

#else
  return "/tmp";
#endif  // defined(__ANDROID__)
}

Status Dir::RecursivelyCreate(absl::string_view pathname) {
  (void)pathname;
  HARD_FAIL("Unimplemented");
}

Status Dir::RecursivelyDelete(absl::string_view pathname) {
  (void)pathname;
  HARD_FAIL("Unimplemented");
}

#endif  // !defined(__APPLE__)

bool File::Exists(const std::string& pathname) {
#if defined(_WIN32)
  DWORD attrs = GetFileAttributesW(Utf8ToWide(pathname).c_str());
  return attrs != INVALID_FILE_ATTRIBUTES;

#else
  struct stat buffer;
  int rc = stat(pathname.c_str(), &buffer);
  return rc == 0;
#endif
}

}  // namespace util
}  // namespace firestore
}  // namespace firebase
