# CMake Version management

cmake_minimum_required(VERSION 3.7)

if (${CMAKE_VERSION} VERSION_LESS 3.18)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else ()
    cmake_policy(VERSION 3.18)
endif ()

# Extract version information from header file

# This assumes that the canonical version is listed in a single line
# like for example: #define BUMP_VERSION 0.1.0
set(VERSION_REGEX "#define BUMP_VERSION[ \t]+\"(.+)\"")

# The PROJECT_SOURCE_DIR variable is not set yet
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/bump/version.h"
        VERSION_STRING REGEX ${VERSION_REGEX})

# Pick out just the version
string(REGEX REPLACE ${VERSION_REGEX} "\\1" VERSION_STRING "${VERSION_STRING}")

# Project configuration

project(Introduction
        VERSION ${VERSION_STRING}
        DESCRIPTION "Command-line utility to bump the semantic version present inside any file."
        HOMEPAGE_URL "https://github.com/hungrybluedev/bump"
        LANGUAGES C
        )

# All targets must follow the C99 standard
set(CMAKE_C_STANDARD 99)
# This property can also be set on a per-target basis.
# However, it is recommended to apply the same standard for all targets
# in order to ensure compatibility and simplicity.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
if (NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wpedantic")
endif ()

add_library("bump_lib-${PROJECT_VERSION}" STATIC
        src/bump/bump.c
        src/bump/fileutil.c
        include/bump/bump.h
        include/bump/fileutil.h
        )
target_include_directories("bump_lib-${PROJECT_VERSION}" PUBLIC include)

# Configuration for the munit testing library
add_library(munit STATIC
        test/src/external/munit/munit.c
        test/include/external/munit/munit.h
        )
set_target_properties(munit PROPERTIES CMAKE_C_STANDARD 11)
target_include_directories(munit PUBLIC test/include/external/munit)

# Configuration for the test executable
add_executable(run-tests test/src/run-tests.c)
target_link_libraries(run-tests PRIVATE "bump_lib-${PROJECT_VERSION}" munit)

# Configuration for the main executable. This is the one we will distribute during release
add_executable(bump src/main.c)
target_include_directories(bump PRIVATE src)
target_link_libraries(bump PRIVATE "bump_lib-${PROJECT_VERSION}")