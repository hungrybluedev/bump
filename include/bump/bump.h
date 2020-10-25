#ifndef BUMP_H
#define BUMP_H

#include <stdio.h>
#include <stdlib.h>

typedef struct version_struct {
  size_t major;
  size_t minor;
  size_t patch;
} Version;

typedef struct line_state_struct {
  const char *input;
  char *output;
  size_t input_index;
  size_t output_index;
  size_t limit;
} LineState;

typedef struct file_state_struct {
  FILE *input;
  FILE *output;
  size_t limit;
  const char *bump_level;
} FileState;

char *initialize_version(Version *version, size_t major, size_t minor, size_t patch);

char *initialize_line_state(LineState *state, const char *input, char *output, size_t limit);

char *initialize_file_state(FileState *state,
                            const char *input_path,
                            const char *output_path,
                            const char *bump_level,
                            size_t limit);

char *bump_major(Version *version);

char *bump_minor(Version *version);

char *bump_patch(Version *version);

char *convert_to_string(Version *version, char *output_buffer, size_t *length);

char *process_line(LineState *state, const char *bump_level);

char *process_file(FileState *state);

#endif//BUMP_H
