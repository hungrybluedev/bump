#ifndef BUMP_FILEUTIL_H
#define BUMP_FILEUTIL_H

#include <stdio.h>
#include <stdbool.h>

bool file_is_valid(const char *input_path, const char *mode);

char *read_line(FILE *input, char *buffer, size_t *length, size_t limit);

#endif//BUMP_FILEUTIL_H