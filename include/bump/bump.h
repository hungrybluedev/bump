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

char *convert_to_string(Version *version, char *output_buffer, size_t *length);

char *process_line(char *line_buffer, const char *input_line, const char *bump_level, size_t *progress);

#endif//BUMP_H