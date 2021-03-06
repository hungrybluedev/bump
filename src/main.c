#include <bump/bump.h>
#include <bump/fileutil.h>
#include <bump/version.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 511
#define INCORRECT_USAGE "Incorrect usage. Type bump --help for more information."
#define INTERMEDIATE_FILE "intermediate.temp"

/**
 * Convert the characters stored in the source string to lowercase and store
 * them in the destination buffer. Additionally, store the number of characters
 * processed in the len pointer.
 *
 * @param destination The buffer to store the lowercase string in.
 * @param source The source string to convert to lowercase.
 * @param len The size_t pointer to store the number of characters processed.
 */
static void convert_to_lowercase_and_store_length(char *destination, const char *source, size_t *len) {
  *len = 0;
  for (char *p = (char *) source; *p; p++, (*len)++) {
    destination[*len] = (char) tolower(*p);
  }
}

static void store_file_name_in(const char *prompt, char *file_name_buffer, bool for_input) {
  bool we_have_file = false;
  while (true) {
    printf("%s", prompt);
    char buffer[MAX_LINE_LENGTH + 1];
    size_t len = 0;
    char *error = read_line(stdin, buffer, &len, MAX_LINE_LENGTH);
    if (error) {
      fprintf(stderr, "Could not read line. The following error occurred: %s\nTry again.\n", error);
      continue;
    }
    we_have_file = file_is_valid(buffer, for_input ? "r" : "w");
    if (we_have_file) {
      memcpy(file_name_buffer, buffer, len + 1);
      break;
    }
    fprintf(stderr, "Could not location file with path: \"%s\"\nTry again.\n", buffer);
  }
}

static void store_bump_level_in(char *bump_level_buffer) {
  bool we_have_bump = false;
  while (true) {
    printf("Enter bump level (major/minor/[patch] or M/m/[p]): ");
    char buffer[MAX_LINE_LENGTH + 1];
    size_t len = 0;
    char *error = read_line(stdin, buffer, &len, MAX_LINE_LENGTH);
    if (error) {
      fprintf(stderr, "Could not read line. The following error occurred: %s\nTry again.\n", error);
      continue;
    }
    if (len == 0) {
      strcpy(bump_level_buffer, "patch");
      we_have_bump = true;
    } else if (len == 1) {
      bool major = buffer[0] == 'M';
      bool minor = buffer[0] == 'm';
      bool patch = buffer[0] == 'p';
      if (major || minor || patch) {
        we_have_bump = true;
      }
      if (major) strcpy(bump_level_buffer, "major");
      if (minor) strcpy(bump_level_buffer, "minor");
      if (patch) strcpy(bump_level_buffer, "patch");
    } else {
      convert_to_lowercase_and_store_length(buffer, buffer, &len);
      if (len == 5 &&
          (strcmp(buffer, "major") == 0 ||
           strcmp(buffer, "minor") == 0 ||
           strcmp(buffer, "patch") == 0)) {
        we_have_bump = true;
        strcpy(bump_level_buffer, buffer);
      }
    }
    if (we_have_bump) {
      break;
    }
    fprintf(stderr, "Could not identify bump level; use major/minor/patch or M/m/p.\nTry again.\n");
  }
}

static bool read_confirmation() {
  while (true) {
    printf("Modify the input file? ([yes]/no or [y]/n): ");

    char buffer[MAX_LINE_LENGTH + 1];
    size_t len = 0;

    char *error = read_line(stdin, buffer, &len, MAX_LINE_LENGTH);
    if (error) {
      fprintf(stderr, "Could not read line. The following error occurred: %s\nTry again.\n", error);
      continue;
    }

    if (len == 0) {
      return true;
    } else if (len == 1) {
      int yn = tolower(buffer[0]);
      if (yn == 'y') {
        return true;
      }
      if (yn == 'n') {
        return false;
      }
    } else {
      convert_to_lowercase_and_store_length(buffer, buffer, &len);
      if (len == 3 && strcmp(buffer, "yes") == 0) {
        return true;
      }
      if (len == 2 && strcmp(buffer, "no") == 0) {
        return false;
      }
    }
    fprintf(stderr, "Could not understand input.\nTry again.\n");
  }
}

static void print_help_message() {
  const char *help_message =
          "Bump semantic version value\n"
          "===========================\n"
          "Version : " BUMP_VERSION "\n"
          "Author  : Subhomoy Haldar (@hungrybluedev)\n"
          "Details : A command-line utility that bumps up the semantic version\n"
          "          number of all instances it detects in a file. It can work\n"
          "          in-place and bump the patch, minor, or major version number.\n\n"
          "Usage   :\n"
          "1. bump\n"
          "          Starts the program in interactive mode.\n"
          "2. bump [--help|-h]\n"
          "          Displays this help message.\n"
          "3. bump [--version|-v]\n"
          "          Shows the current version of this program.\n"
          "4. bump [--input|-i] path/to/file.txt [[--level|-l] [major|minor|patch]]? \\\n"
          "        [[--output|-o] path/to/output_file.txt]?\n"
          "          Performs the processing on the file path provided if it exists.\n\n"
          "          The level switch and value is optional. The values allowed are:\n"
          "          a. patch or p - a.b.c -> a.b.(c + 1)\n"
          "          b. minor or m - a.b.c -> a.(b + 1).0\n"
          "          c. major or M - a.b.c -> (a + 1).0.0\n\n"
          "          The default level is \"patch\".\n\n"
          "          The output switch and value pair is also optional. By default\n"
          "          the result is stored in-place, in the input file.";
  printf("%s\n", help_message);
}


static char *process_input_path_value(char *input_file_name,
                                      bool *we_have_input_path,
                                      const char *file_path) {
  if (*we_have_input_path) {
    return "Repeated input file switch.";
  }
  if (!file_is_valid(file_path, "r")) {
    return "The input file path provided is not valid.";
  }
  strcpy(input_file_name, file_path);
  *we_have_input_path = true;
  return NULL;
}

static char *process_output_path_value(char *output_file_name,
                                       bool *we_have_output_path,
                                       const char *file_path) {
  if (*we_have_output_path) {
    return "Repeated output file switch.";
  }
  if (!file_is_valid(file_path, "w")) {
    return "The output file path provided is not valid.";
  }
  strcpy(output_file_name, file_path);
  *we_have_output_path = true;
  return NULL;
}

static char *process_single_switch(const char *switch_value) {
  char command[MAX_LINE_LENGTH] = {0};
  size_t len;

  convert_to_lowercase_and_store_length(command, switch_value, &len);

  if (len == 2) {
    // We have an abbreviated switch. It must begin with a '-'
    if (command[0] != '-') {
      return INCORRECT_USAGE;
    }

    switch (command[1]) {
      case 'h':
        print_help_message();
        return NULL;
      case 'v':
        printf("Bump version : %s\n", BUMP_VERSION);
        return NULL;
      default:
        return INCORRECT_USAGE;
    }
  }
  if (len == 6 && strcmp(command, "--help") == 0) {
    print_help_message();
    return NULL;
  }
  if (len == 9 && strcmp(command, "--version") == 0) {
    printf("Bump version : %s\n", BUMP_VERSION);
    return NULL;
  }

  // If we reached here, we must have had no matching switch
  return INCORRECT_USAGE;
}

static char *process_bump_value(char *bump_level,
                                bool *we_have_bump_value,
                                const char *bump_level_argument) {

  char buffer[MAX_LINE_LENGTH + 1];
  size_t value_len;
  convert_to_lowercase_and_store_length(buffer, bump_level_argument, &value_len);

  if (*we_have_bump_value) {
    return "Repeated patch level switch.";
  }
  if (value_len == 1) {
    switch (bump_level_argument[0]) {
      case 'p':
        // No need to copy, it is already the default.
        break;
      case 'm':
        strcpy(bump_level, "minor");
        break;
      case 'M':
        strcpy(bump_level, "major");
        break;
      default:
        return "Unrecognised patch value. Valid levels are major(M), minor(m), or patch(p).";
    }
  } else if (value_len == 5) {
    if (strcmp(bump_level_argument, "major") == 0) {
      strcpy(bump_level, "major");
    } else if (strcmp(bump_level_argument, "minor") == 0) {
      strcpy(bump_level, "minor");
    } else if (strcmp(bump_level_argument, "patch") == 0) {
      // No need to copy
    } else {
      return "Unrecognised patch value. Valid levels are major(M), minor(m), or patch(p).";
    }
  }
  *we_have_bump_value = true;
  return NULL;
}

int main(int argc, char const *argv[]) {
  char input_file_name[MAX_LINE_LENGTH + 1] = {0};
  char output_file_name[MAX_LINE_LENGTH + 1] = {0};
  char bump_level[MAX_LINE_LENGTH + 1] = {0};

  char *error;

  // Process command-line arguments
  if (argc == 1) {
    // There are no arguments. We need to obtain the values required through user input.
    store_file_name_in("Enter file name to process : ", input_file_name, true);
    store_bump_level_in(bump_level);
    if (read_confirmation()) {
      strcpy(output_file_name, input_file_name);
    } else {
      store_file_name_in("Enter output file name : ", output_file_name, false);
    }
  } else if (argc == 2) {
    // Help and version commands.
    // In all cases, the program will never execute code afterwards outside this block.
    error = process_single_switch(argv[1]);
    if (error) {
      fprintf(stderr, "%s\n", error);
      return EXIT_FAILURE;
    } else {
      return EXIT_SUCCESS;
    }

  } else if (argc % 2 == 0) {
    // The pairs are mismatched
    fprintf(stderr, "%s\n", INCORRECT_USAGE);
    return EXIT_FAILURE;
  } else {
    // The parameters should be present in pairs.
    // The count must be odd because the name of the executable is the first argument.
    bool we_have_input_path = false;
    bool we_have_bump_value = false;
    bool we_have_output_path = false;

    strcpy(bump_level, "patch");


    // Iterate pair wise. We do not care about the order in which
    // the pairs appear. We are only interested in the values, and
    // if they are valid for the switch given.
    size_t count = (size_t) argc;
    for (size_t index = 1; index < count; index += 2) {
      // The first of the pair should be a switch
      if (argv[index][0] != '-') {
        fprintf(stderr, "%s\n", INCORRECT_USAGE);
        return EXIT_FAILURE;
      }

      size_t arg_len = strlen(argv[index]);

      if (arg_len == 2) {
        // Abbreviated switch
        switch (argv[index][1]) {
          case 'i':
            error = process_input_path_value(input_file_name, &we_have_input_path, argv[index + 1]);
            if (error) {
              fprintf(stderr, "%s\n", error);
              return EXIT_FAILURE;
            }
            break;
          case 'l':
            error = process_bump_value(bump_level, &we_have_bump_value, argv[index + 1]);
            if (error) {
              fprintf(stderr, "%s\n", error);
              return EXIT_FAILURE;
            }
            break;
          case 'o':
            error = process_output_path_value(output_file_name, &we_have_output_path, argv[index + 1]);
            if (error) {
              fprintf(stderr, "%s\n", error);
              return EXIT_FAILURE;
            }
            break;
          default:
            fprintf(stderr, "%s\n", error);
            return EXIT_FAILURE;
        }
      } else {
        // Switch is not abbreviated
        if (strcmp(argv[index], "--input") == 0) {
          error = process_input_path_value(input_file_name, &we_have_input_path, argv[index + 1]);
          if (error) {
            fprintf(stderr, "%s\n", error);
            return EXIT_FAILURE;
          }
        } else if (strcmp(argv[index], "--level") == 0) {
          error = process_bump_value(bump_level, &we_have_bump_value, argv[index + 1]);
          if (error) {
            fprintf(stderr, "%s\n", error);
            return EXIT_FAILURE;
          }
        } else if (strcmp(argv[index], "--output") == 0) {
          error = process_output_path_value(output_file_name, &we_have_output_path, argv[index + 1]);
          if (error) {
            fprintf(stderr, "%s\n", error);
            return EXIT_FAILURE;
          }
        }
      }
    }
    if (!we_have_input_path) {
      fprintf(stderr, "Input file not specified.\n");
      return EXIT_FAILURE;
    }
    if (!we_have_output_path) {
      strcpy(output_file_name, input_file_name);
    }
  }

  bool inplace = strcmp(input_file_name, output_file_name) == 0;
  if (inplace) {
    // Check if we can create the file for writing
    if (!file_is_valid(INTERMEDIATE_FILE, "w")) {
      fprintf(stderr, "%s\n", "Could not create temporary file for writing.");
      return EXIT_FAILURE;
    }

    // Mark the output as the temporary.
    strcpy(output_file_name, INTERMEDIATE_FILE);
  }

  FileState state = {0};
  initialize_file_state(&state, input_file_name, output_file_name, bump_level, MAX_LINE_LENGTH);

  error = process_file(&state);
  if (error) {
    fprintf(stderr, "Error occurred while trying to process file: %s\n", error);
  }

  if (inplace) {
    int c;
    FILE *temp = fopen(INTERMEDIATE_FILE, "r");
    if (!temp) {
      fprintf(stderr, "Could not open temporary file for reading.\n");
      return EXIT_FAILURE;
    }
    FILE *dump = fopen(input_file_name, "w");
    if (!dump) {
      fprintf(stderr, "Could not open final output file for writing.\n");
      return EXIT_FAILURE;
    }

    while ((c = fgetc(temp)) != EOF) {
      fputc(c, dump);
    }
    if (ferror(temp)) {
      fprintf(stderr, "I/O error occurred while trying to read from temporary file.\n");
      return EXIT_FAILURE;
    }
    if (ferror(dump)) {
      fprintf(stderr, "I/O error occurred while trying to write to output file.\n");
      return EXIT_FAILURE;
    }

    int e;
    e = fclose(temp);
    if (e) {
      fprintf(stderr, "Unable to close temporary file.\n");
      return EXIT_FAILURE;
    }
    e = fclose(dump);
    if (e) {
      fprintf(stderr, "Unable to close output file.\n");
      return EXIT_FAILURE;
    }
    e = remove(INTERMEDIATE_FILE);
    if (e) {
      fprintf(stderr, "Unable to delete temporary file.\n");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
