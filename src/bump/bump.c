#include <bump/bump.h>
#include <limits.h>
#include <stdlib.h>

char *initialize_version(Version *version, const size_t major, const size_t minor, const size_t patch) {
  if (!version) {
    return "Empty pointer received.";
  }
  version->major = major;
  version->minor = minor;
  version->patch = patch;
  return NULL;
}

static char *sanity_check(Version *version) {
  if (!version) {
    return "Empty pointer received.";
  }
  if (version->major == SIZE_MAX) {
    return "Major version number is too big.";
  }
  if (version->minor == SIZE_MAX) {
    return "Minor version number is too big.";
  }
  if (version->patch == SIZE_MAX) {
    return "Patch version number is too big.";
  }
  return NULL;
}

char *bump_major(Version *version) {
  char *message = sanity_check(version);
  if (message) {
    return message;
  }
  version->major++;
  version->minor = 0;
  version->patch = 0;
  return NULL;
}

char *bump_minor(Version *version) {
  char *message = sanity_check(version);
  if (message) {
    return message;
  }
  version->minor++;
  version->patch = 0;
  return NULL;
}

char *bump_patch(Version *version) {
  char *message = sanity_check(version);
  if (message) {
    return message;
  }
  version->patch++;
  return NULL;
}
