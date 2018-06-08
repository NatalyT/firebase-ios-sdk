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

static bool Matches(NSError* error, NSErrorDomain domain, NSInteger code) {
  return [error.domain isEqual:domain] && error.code == code;
}

std::string Dir::TempDir() {
  const char* env_tmpdir = getenv("TMPDIR");
  if (env_tmpdir) {
    return env_tmpdir;
  }

  NSString* ns_tmpdir = NSTemporaryDirectory();
  if (ns_tmpdir) {
    return MakeString(ns_tmpdir);
  }

  return "/tmp";
}

Status Dir::RecursivelyCreate(absl::string_view pathname) {
  NSString* ns_path = WrapNSString(pathname);

  NSError* error = nil;
  if (![[NSFileManager defaultManager] createDirectoryAtPath:ns_path
                                 withIntermediateDirectories:YES
                                                  attributes:nil
                                                       error:&error]) {
    return Status::FromNSError(error);
  }
  return Status::OK();
}

Status Dir::RecursivelyDelete(absl::string_view pathname) {
  NSString* ns_path = WrapNSString(pathname);
  NSError* error = nil;
  if (![[NSFileManager defaultManager] removeItemAtPath:ns_path error:&error]) {
    if (Matches(error, NSCocoaErrorDomain, NSFileNoSuchFileError)) {
      // Successful by definition
      return Status::OK();
    }

    return Status::FromNSError(error);
  }
  return Status::OK();
}

}  // namespace util
}  // namespace firestore
}  // namespace firebase
