#include <bump/fileutil.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>

bool file_is_valid(const char *input_path, const char *mode) {
  FILE *input_file = fopen(input_path, mode);
  bool result = input_file != NULL;
  if (result) {
    fclose(input_file);
  }
  return result;
}

static char *validate(FILE *input, const char *buffer) {
  if (!input) {
    return "Empty pointer for input file.";
  }
  if (!buffer) {
    return "Storage buffer is empty.";
  }
  return NULL;
}

char *read_line(FILE *input, char *buffer, size_t *length, size_t limit) {
  char *error = validate(input, buffer);
  if (error) {
    return error;
  }

  int ch;
  *length = 0;
  memset(buffer, 0, limit);

  while (*length < limit && (ch = fgetc(input)) != '\n') {
    if (ch == EOF) {
      return "End of file reached";
    }
    buffer[*length] = (char) ch;
    (*length)++;
  }
  buffer[*length] = '\0';

  return NULL;
}