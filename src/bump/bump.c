#include <bump/bump.h>
#include <bump/fileutil.h>
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

char *initialize_line_state(LineState *state, const char *input, char *output, const size_t limit) {
  if (!state) {
    return "Empty pointer received for state";
  }
  if (!input) {
    return "Input buffer is null";
  }
  if (!output) {
    return "Output buffer is null";
  }
  state->input = input;
  state->output = output;
  state->input_index = 0;
  state->output_index = 0;
  // The minimum length needed for a version string is 5.
  // If we have a string length limit smaller than that, we clamp the limit to 0.
  state->limit = limit < 5 ? 0 : limit;
  return NULL;
}

static void keep_going(LineState *state) {
  if (state->input_index == state->limit) {
    return;
  }
  state->output[state->output_index] = state->input[state->input_index];
  state->input_index++;
  state->output_index++;
}

static size_t extract_decimal_number(LineState *state) {
  char c;
  size_t value = 0;
  while (state->input_index < state->limit && isdigit(c = state->input[state->input_index])) {
    value *= 10;
    value += c - '0';
    state->input_index++;
  }
  return value;
}

char *process_line(LineState *state, const char *bump_level) {
  if (!state) {
    return "Null value received for state";
  }
  if (state->limit == 0 || state->input_index == state->limit) {
    return NULL;
  }

  while (state->input_index < state->limit) {
    char c = state->input[state->input_index];
    if (isdigit(c)) {
      // Start a greedy search for the pattern of "x.y.z" where x, y, and z are decimal numbers
      size_t major = extract_decimal_number(state);
      c = state->input[state->input_index];

      if (c != '.') {
        // We have a normal number. Dump it to the output and proceed normally.
        int chars_printed = sprintf(state->output + state->output_index, "%zu", major);
        if (chars_printed < 1) {
          return "Error occurred while trying to write to output buffer";
        }
        state->output_index += chars_printed;
        keep_going(state);
        continue;
      }
      state->input_index++;
      if (state->input_index == state->limit) {
        return NULL;
      }
      c = state->input[state->input_index];

      if (!isdigit(c)) {
        // We have a x. followed by a non-digit
        keep_going(state);
        continue;
      }

      size_t minor = extract_decimal_number(state);
      if (state->input_index == state->limit) {
        return NULL;
      }
      c = state->input[state->input_index];


      if (c != '.') {
        // We have an input of the form x.y only. No period follows the y
        int chars_printed = sprintf(state->output + state->output_index, "%zu.%zu", major, minor);
        if (chars_printed < 1) {
          return "Error occurred while trying to write to output buffer";
        }
        state->output_index += chars_printed;
        keep_going(state);
        continue;
      }
      state->input_index++;
      c = state->input[state->input_index];

      if (!isdigit(c)) {
        // We have a x.y. followed by a non-digit
        keep_going(state);
        continue;
      }

      size_t patch = extract_decimal_number(state);
      c = state->input[state->input_index];

      if (c == '.') {
        // We have x.y.z. which is invalid.
        int chars_printed = sprintf(state->output + state->output_index, "%zu.%zu.%zu", major, minor, patch);
        if (chars_printed < 1) {
          return "Error occurred while trying to write to output buffer";
        }
        state->output_index += chars_printed;
        keep_going(state);
        continue;
      }

      // We now have all three numbers.

      Version version = {0};
      initialize_version(&version, major, minor, patch);

      if (strcmp(bump_level, "major") == 0) {
        bump_major(&version);
      } else if (strcmp(bump_level, "minor") == 0) {
        bump_minor(&version);
      } else if (strcmp(bump_level, "patch") == 0) {
        bump_patch(&version);
      } else {
        return "Invalid bump level";
      }

      size_t version_len;
      char *error = convert_to_string(&version, state->output + state->output_index, &version_len);
      state->output_index += version_len;

      if (error) {
        return error;
      }

      if (state->input_index < state->limit) {
        strcpy(state->output + state->output_index, state->input + state->input_index);
      }

      // We are done so we exit early
      return NULL;
    } else {
      keep_going(state);
    }
  }
  return NULL;
}
