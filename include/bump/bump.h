#ifndef BUMP_H
#define BUMP_H

#include <stdio.h>
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

char *process_line(char *output_line,
                   const char *input_line,
                   const char *bump_level,
                   size_t *input_offset,
                   size_t *output_offset);

char *process_file(FILE *input_stream,
                   FILE *output_stream,
                   const char *bump_level,
                   size_t line_limit);

#endif//BUMP_H