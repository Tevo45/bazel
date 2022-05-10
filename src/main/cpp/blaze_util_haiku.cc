#include <spawn.h>

#include <FindDirectory.h>
#include <Path.h>

#include <image.h>

// TODO filter this out later
#include "src/main/cpp/blaze_util.h"
#include "src/main/cpp/blaze_util_platform.h"
#include "src/main/cpp/util/errors.h"
#include "src/main/cpp/util/exit_code.h"
#include "src/main/cpp/util/file.h"
#include "src/main/cpp/util/file_platform.h"
#include "src/main/cpp/util/logging.h"
#include "src/main/cpp/util/path.h"
#include "src/main/cpp/util/port.h"
#include "src/main/cpp/util/strings.h"

namespace blaze {

using std::string;

string GetOutputRoot() {
  // I can get rid of -lbe if I use the C API here...
  BPath path;
  if(find_directory(B_USER_CACHE_DIRECTORY, &path) == B_OK) {
    return blaze_util::JoinPath(path.Path(), "bazel");
  } else {
    return "/tmp";
  }
}

void WarnFilesystemType(const blaze_util::Path &output_base) {
  // Stub
}

string GetSelfPath(const char* argv0) {
  int32 cookie = 0;
  image_info info;

  while(get_next_image_info(0, &cookie, &info) == B_OK)
    if(info.type == B_APP_IMAGE)
      return string(info.name);

  // Should never happen.
  BAZEL_DIE(blaze_exit_code::INTERNAL_ERROR)
      << "Unable to determine the location of this Bazel executable.";
  return "";
}

uint64_t GetMillisecondsMonotonic() {
  struct timespec ts = {};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000LL + (ts.tv_nsec / 1000000LL);
}

void SetScheduling(bool batch_cpu_scheduling, int io_nice_level) {
  // Stub
}

std::unique_ptr<blaze_util::Path> GetProcessCWD(int pid) {
  // TODO
  return nullptr;
}

bool IsSharedLibrary(const string &filename) {
  return blaze_util::ends_with(filename, ".so");
}

string GetSystemJavabase() {
  // If JAVA_HOME is defined, then use it as default.
  string javahome = GetPathEnv("JAVA_HOME");

  if (!javahome.empty()) {
    string javac = blaze_util::JoinPath(javahome, "bin/javac");
    if (access(javac.c_str(), X_OK) == 0) {
      return javahome;
    }
    BAZEL_LOG(WARNING)
        << "Ignoring JAVA_HOME, because it must point to a JDK, not a JRE.";
  }

  return "/system/lib/openjdk11";
}

int ConfigureDaemonProcess(posix_spawnattr_t *attrp,
                           const StartupOptions &options) {
  // TODO is there anything to configure?
  return 0;
}

void WriteSystemSpecificProcessIdentifier(const blaze_util::Path &server_dir,
                                          pid_t server_pid)
{
  // TODO what to do here?
}

bool VerifyServerProcess(int pid, const blaze_util::Path &output_base) {
  // TODO see blaze_util_bsd.cc
  return killpg(pid, 0) == 0;
}

void ExcludePathFromBackup(const blaze_util::Path &path) {
  // TODO is there anything to do here?
}

int32_t GetExplicitSystemLimit(const int resource) {
  // TODO ???
  return -1;
}

}  // namespace blaze
