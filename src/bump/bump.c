#include <bump/bump.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
char *convert_to_string(Version *version, char *output_buffer, size_t *length) {
  if (!version) {
    return "Empty version pointer value.";
  }
  if (!output_buffer) {
    return "Empty output buffer pointer.";
  }
  *length = 0;
  int count = sprintf(output_buffer,
                      "%zu.%zu.%zu",
                      version->major,
                      version->minor,
                      version->patch);
  if (count < 0) {
    return "Error occurred while trying to write to string buffer.";
  }
  *length = (size_t) count;
  return NULL;
}

char *process_line(char *line_buffer,
                   const char *input_line,
                   const char *bump_level,
                   size_t *progress) {
  if (!line_buffer) {
    return "Empty output buffer.";
  }
  if (!input_line) {
    return "Empty input string.";
  }
  if (!bump_level) {
    return "Empty bump level.";
  }
  bool found_major = false;
  bool found_minor = false;
  bool found_patch = false;

  size_t major, minor, patch;

  size_t current_value = 0;
  ssize_t last_non_digit_index = -1;
  ssize_t version_starting_index = -1;

  size_t limit = strlen(input_line);
  size_t index = 0;

  for (; index < limit; ++index) {
    char c = input_line[index];
    if (isdigit(c)) {
      current_value *= 10;
      current_value += c - '0';
    } else if (input_line[index] == '.') {
      // A number has just ended with a '.'
      // See if our criteria match
      if (!found_major) {
        // Make sure that the digit was not preceded by a '.'
        if (last_non_digit_index != -1 && input_line[last_non_digit_index] == '.') {
          found_major = false;
          line_buffer[index] = input_line[index];
        } else {
          found_major = true;
          major = current_value;
          version_starting_index = last_non_digit_index + 1;
        }
      } else if (!found_minor) {
        // This time, we want to have a '.' precede the number
        if (last_non_digit_index != -1 && input_line[last_non_digit_index] != '.') {
          found_major = false;
          // We didn't find it. We do not want to change values such as
          // a.b.c.d -> a.b'.c'.d' so we don't consider the previous
          // value as major.
          line_buffer[index] = input_line[index];
        } else {
          found_minor = true;
          minor = current_value;
        }
      }
      current_value = 0;
      last_non_digit_index = index;
    } else {
      if (found_major && found_minor) {
        // Again, we want to have a '.' precede the number
        if (input_line[last_non_digit_index] != '.') {
          found_major = false;
          found_minor = false;
          line_buffer[index] = input_line[index];
        } else {
          patch = current_value;
          found_patch = true;
          break;
        }
      } else {
        // We simply copy over the characters we are not interested in.
        line_buffer[index] = input_line[index];
      }
      current_value = 0;
      last_non_digit_index = index;
    }
  }

  if (!found_patch && index == limit) {
    // We reached the length. So the patch value is still unassigned
    patch = current_value;
  }

  if (!found_major) {
    return NULL;
  }

  Version version = {0};
  size_t version_len;

  initialize_version(&version, major, minor, patch);

  if (strcmp(bump_level, "major") == 0) {
    bump_major(&version);
  } else if (strcmp(bump_level, "minor") == 0) {
    bump_minor(&version);
  } else if (strcmp(bump_level, "patch") == 0) {
    bump_patch(&version);
  } else {
    return "Invalid bump version.";
  }

  char *error = convert_to_string(&version, &line_buffer[version_starting_index], &version_len);
  if (error) {
    return error;
  }

  if (index < limit) {
    strcpy(&line_buffer[version_starting_index + version_len], &input_line[index]);
  }

  if (progress) {
    *progress = version_starting_index + version_len;
  }
  return NULL;
}