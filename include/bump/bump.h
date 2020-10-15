#ifndef BUMP_H
#define BUMP_H

#include <stdlib.h>

typedef struct version_struct {
  size_t major;
  size_t minor;
  size_t patch;
} Version;

char *initialize_version(Version *version, size_t major, size_t minor, size_t patch);

char *bump_major(Version *version);

char *bump_minor(Version *version);

char *bump_patch(Version *version);


#endif//BUMP_H