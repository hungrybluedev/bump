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

  munit_assert_null(process_line(line, input_line, "patch", NULL));
  munit_assert_string_equal(line, "v0.1.2");

  return MUNIT_OK;
}

MunitResult process_line_1_1_51() {
  char line[21] = {0};
  const char *input_line = "1.1.51 is the version";

  munit_assert_null(process_line(line, input_line, "minor", NULL));
  munit_assert_string_equal(line, "1.2.0 is the version");

  return MUNIT_OK;
}

MunitResult process_two() {
  char line[42] = {0};
  const char *input_line = "First we have 1.6.84, then we have 8.16.3!";

  printf("\n\t1. Before first extraction.\n");

  size_t offset = 0;
  munit_assert_null(process_line(line, input_line, "patch", &offset));
  munit_assert_string_equal(line, "First we have 1.6.85, then we have 8.16.3!");
  printf("\t2. After first extraction.\n");

  char *new_line = &line[offset];

  printf("\t3. Before second extraction.\n");

  munit_assert_null(process_line(new_line, new_line, "major", &offset));
  munit_assert_string_equal(line, "First we have 1.6.85, then we have 9.0.0!");
  printf("\t4. After second extraction.\n");

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