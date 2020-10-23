#include <stdio.h>

#include <bump/bump.h>
#include <bump/version.h>

#include <munit.h>

/*
 *
 * UNIT TEST FUNCTIONS
 * ===================
 *
 * All unit tests are of the form:
 *
 * MunitResult <test_name>(const MunitParameter params[],
 *                         void *user_data_or_fixture) {
 *   // perform tests.
 *   // use the munit_assert_... macros.
 *   return MUNIT_OK;
 * }
 *
 * It is necessary for the unit test functions to be added to
 * the `test` array.
 */

MunitResult bump_patch_0_0_1() {
  Version version = {0};
  munit_assert_null(initialize_version(&version, 0, 0, 1));

  bump_patch(&version);

  munit_assert(version.major == 0);
  munit_assert(version.minor == 0);
  munit_assert(version.patch == 2);

  return MUNIT_OK;
}

MunitResult bump_patch_0_0_15() {
  Version version = {0};
  munit_assert_null(initialize_version(&version, 0, 0, 15));

  bump_patch(&version);

  munit_assert(version.major == 0);
  munit_assert(version.minor == 0);
  munit_assert(version.patch == 16);

  return MUNIT_OK;
}

MunitResult bump_minor_0_0_23() {
  Version version = {0};
  munit_assert_null(initialize_version(&version, 0, 0, 23));

  bump_minor(&version);

  munit_assert(version.major == 0);
  munit_assert(version.minor == 1);
  munit_assert(version.patch == 0);

  return MUNIT_OK;
}

MunitResult bump_major_0_2_8() {
  Version version = {0};
  munit_assert_null(initialize_version(&version, 0, 2, 8));

  bump_major(&version);

  munit_assert(version.major == 1);
  munit_assert(version.minor == 0);
  munit_assert(version.patch == 0);

  return MUNIT_OK;
}

MunitResult convert_0_0_1() {
  char line[16];
  Version version = {0};
  munit_assert_null(initialize_version(&version, 0, 0, 1));
  size_t len;

  munit_assert_null(convert_to_string(&version, line, &len));
  munit_assert_string_equal(line, "0.0.1");
  munit_assert(len == 5);

  return MUNIT_OK;
}

MunitResult convert_4_23_56() {
  char line[16];
  Version version = {0};
  munit_assert_null(initialize_version(&version, 4, 23, 56));
  size_t len;

  munit_assert_null(convert_to_string(&version, line, &len));
  munit_assert_string_equal(line, "4.23.56");
  munit_assert(len == 7);

  return MUNIT_OK;
}

MunitResult process_line_0_1_1() {
  char line[10] = {0};
  const char *input_line = "v0.1.1";

  LineState state = {0};
  initialize_line_state(&state, input_line, line, strlen(input_line));

  munit_assert_null(process_line(&state, "patch"));
  munit_assert_string_equal(line, "v0.1.2");
  munit_assert_size(state.input_index, ==, 6);
  munit_assert_size(state.output_index, ==, 6);

  return MUNIT_OK;
}

MunitResult process_line_1_1_51() {
  char line[40] = {0};
  const char *input_line = "2.5 is a number 1.1.51 is the version";

  LineState state = {0};
  initialize_line_state(&state, input_line, line, strlen(input_line));

  munit_assert_null(process_line(&state, "minor"));
  munit_assert_string_equal(line, "2.5 is a number 1.2.0 is the version");
  munit_assert_size(state.input_index, ==, 22);
  munit_assert_size(state.output_index, ==, 21);

  return MUNIT_OK;
}

MunitResult process_two() {
  char line[50] = {0};
  char buffer[50] = {0};
  const char *input_line = "First we (12) have 1.6.84, then we have 8.16.3!";

  LineState state = {0};
  initialize_line_state(&state, input_line, line, strlen(input_line));

  munit_assert_null(process_line(&state, "patch"));
  munit_assert_string_equal(line, "First we (12) have 1.6.85, then we have 8.16.3!");
  munit_assert_size(state.input_index, ==, 25);
  munit_assert_size(state.output_index, ==, 25);

  strcpy(buffer, line);

  munit_assert_null(process_line(&state, "major"));
  munit_assert_string_equal(line, "First we (12) have 1.6.85, then we have 9.0.0!");
  munit_assert_size(state.input_index, ==, 46);
  munit_assert_size(state.output_index, ==, 45);

  return MUNIT_OK;
}

MunitResult process_test_cases() {
  // Things to update:
  // 1. the `count` variable - stores the number of test cases
  // 2. the `input_lines` array - stores the input lines
  // 3. the `expected_lines` array - stores 3 types of outputs for each input line. The order is patch, minor, major
  const size_t count = 5;// <- update this
  const size_t max_line_width = 256;
  char line[max_line_width];
  char copy[max_line_width];

  const char *bump_levels[] = {"patch", "minor", "major"};

  const char *input_lines[] = {
          "<modelVersion>4.0.0</modelVersion>",
          "#define VERSION \"0.3.56\"",
          "2.5 is a number but 1.0.7 is a version",
          "8.0. is not a version",
          "Let's put one at the end 9.",
  };
  //      ^
  //      +-- Add new input lines at the end
  const char *expected_lines[] = {
          "<modelVersion>4.0.1</modelVersion>",
          "<modelVersion>4.1.0</modelVersion>",
          "<modelVersion>5.0.0</modelVersion>",
          "#define VERSION \"0.3.57\"",
          "#define VERSION \"0.4.0\"",
          "#define VERSION \"1.0.0\"",
          "2.5 is a number but 1.0.8 is a version",
          "2.5 is a number but 1.1.0 is a version",
          "2.5 is a number but 2.0.0 is a version",
          "8.0. is not a version",
          "8.0. is not a version",
          "8.0. is not a version",
          "Let's put one at the end 9.",
          "Let's put one at the end 9.",
          "Let's put one at the end 9.",
  };
  //      ^
  //      +-- Add the three variations of the outputs at the end. Remember, the order is
  //          patch, minor, major

  for (size_t index = 0; index < count; ++index) {
    strcpy(line, input_lines[index]);

    for (size_t bump_index = 0; bump_index < 3; ++bump_index) {
      memset(copy, 0, max_line_width);

      LineState state = {0};
      munit_assert_null(initialize_line_state(&state, line, copy, max_line_width));

      while (state.input_index < state.limit) {
        process_line(&state, bump_levels[bump_index]);
      }

      munit_assert_string_equal(expected_lines[index * 3 + bump_index], copy);
    }
  }

  return MUNIT_OK;
}

/*
 * MUNIT TEST CONFIGURATION
 * ========================
 *
 * Boilerplate code for the munit testing library.
 * The last NULL test item acts as a sentinel.
 */

MunitTest tests[] = {
        {"/bump_patch_0_0_1", bump_patch_0_0_1, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/bump_patch_0_0_15", bump_patch_0_0_15, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/bump_minor_0_0_23", bump_minor_0_0_23, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/bump_major_0_2_8", bump_major_0_2_8, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/convert_0_0_1", convert_0_0_1, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/convert_4_23_56", convert_4_23_56, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/process_line_0_1_1", process_line_0_1_1, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/process_line_1_1_51", process_line_1_1_51, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/process_two", process_two, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {"/process_test_cases", process_test_cases, NULL,
         NULL, MUNIT_TEST_OPTION_NONE, NULL},
        {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite suite = {"/bump-test-suite", tests, NULL, 1,
                                 MUNIT_SUITE_OPTION_NONE};

/*
 * MAIN FUNCTION
 * =============
 */

int main(int argc, char *argv[]) {
  printf("Bump project version: %s\n\n", BUMP_VERSION);
  return munit_suite_main(&suite, NULL, argc, argv);
}