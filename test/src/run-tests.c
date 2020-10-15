#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

MunitResult bump_patch_0_0_1(const MunitParameter ignored_params[],
                             void *ignored_data) {

  Version version = {0};
  initialize_version(&version, 0, 0, 1);

  bump_patch(&version);

  munit_assert(version.major == 0);
  munit_assert(version.minor == 0);
  munit_assert(version.patch == 2);

  return MUNIT_OK;
}

MunitResult bump_patch_0_0_15(const MunitParameter ignored_params[],
                              void *ignored_data) {

  Version version = {0};
  initialize_version(&version, 0, 0, 15);

  bump_patch(&version);

  munit_assert(version.major == 0);
  munit_assert(version.minor == 0);
  munit_assert(version.patch == 16);

  return MUNIT_OK;
}

MunitResult bump_minor_0_0_23(const MunitParameter ignored_params[],
                              void *ignored_data) {

  Version version = {0};
  initialize_version(&version, 0, 0, 23);

  bump_minor(&version);

  munit_assert(version.major == 0);
  munit_assert(version.minor == 1);
  munit_assert(version.patch == 0);

  return MUNIT_OK;
}

MunitResult bump_major_0_2_8(const MunitParameter ignored_params[],
                             void *ignored_data) {

  Version version = {0};
  initialize_version(&version, 0, 2, 8);

  bump_major(&version);

  munit_assert(version.major == 1);
  munit_assert(version.minor == 0);
  munit_assert(version.patch == 0);

  return MUNIT_OK;
}

/*
 * MUNIT TEST CONFIGURATION
 * ========================
 *
 * Boilerplate code for the munit testing library.
 *
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