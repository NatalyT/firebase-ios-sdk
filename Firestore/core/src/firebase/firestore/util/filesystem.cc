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

Status CreateDir(absl::string_view pathname) {
  (void)pathname;
  HARD_FAIL("Unimplemented");
}

bool Exists(const std::string& pathname) {
  struct stat buffer;
  int rc = stat(pathname.c_str(), &buffer);
  return rc == 0;
}

#if !defined(__APPLE__)
std::string GetTempDir() {
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

Status RecursivelyCreateDir(absl::string_view pathname) {
  (void)pathname;
  HARD_FAIL("Unimplemented");
}

Status RecursivelyDelete(absl::string_view pathname) {
  (void)pathname;
  HARD_FAIL("Unimplemented");
}

#endif  // !defined(__APPLE__)

}  // namespace util
}  // namespace firestore
}  // namespace firebase
