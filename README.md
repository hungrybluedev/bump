# ⬆️ bump

## 🎖️ Badges

<div align="center" style="width: 60%; margin: 0 auto;">

| Description |                                                                                                                                       Badges                                                                                                                                        |
| :---------: | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|    LGTM     | [![LGTM Grade](https://img.shields.io/lgtm/grade/cpp/github/hungrybluedev/bump)](https://lgtm.com/projects/g/hungrybluedev/bump/context:cpp) [![LGTM Alerts](https://img.shields.io/lgtm/alerts/github/hungrybluedev/bump)](https://lgtm.com/projects/g/hungrybluedev/bump/alerts/) |
|   License   |                                                                                                   [![GitHub](https://img.shields.io/github/license/hungrybluedev/bump)](LICENSE)                                                                                                    |
|    CI/CD    |              [![CI for release tags](https://github.com/hungrybluedev/bump/workflows/CI%20for%20release%20tags/badge.svg) ![Main CI test](https://github.com/hungrybluedev/bump/workflows/Main%20CI%20test/badge.svg)](https://github.com/hungrybluedev/bump/actions)               |
|   Version   |                                                                  [![GitHub release (latest by date)](https://img.shields.io/github/v/release/hungrybluedev/bump)](https://github.com/hungrybluedev/bump/releases)                                                                   |

</div>

## 👋 Introduction

A cross-platform command-line utility that bumps the (full) semantic version numbers present in a file according to a given bump level.

## 🌟 Features

- 🚀 Performant yet simple - written entirely in C.
- 🌏 Cross-platform - works on Windows, MacOS and Linux.
- 🌞 Permissively licensed under the [MIT License](LICENSE).

## 📝 Instructions

1. `bump` is a single executable file. You can run it from the folder it is contained it, or add it to your path.
2. You can use it in silent mode using command-line arguments or in interactive mode.

This is the list of command-line arguments available:

| Switch         | Description                                                                   |
| -------------- | ----------------------------------------------------------------------------- |
| `--help -h`    | Display the help message                                                      |
| `--version -v` | Display the version of the executable                                         |
| `--input -i`   | Switch preceding the input file name (required)                               |
| `--level -l`   | Optional switch preceding the bump level (major - M, minor - m, or patch - p) |
| `--output -o`  | Optional switch preceding the output file name                                |

⚠️ Note that the file paths will be calculated relative to your working directory. If unsure, use absolute paths; they are guaranteed to work.

## 📂 Installation

ℹ️ _Install scripts and support for package managers coming soon._

You can download the pre-built binaries from the [Releases](https://github.com/hungrybluedev/bump/releases/) page. The ZIP files contain executables built for the corresponding operating systems. Extract the binary, (optionally) place it in your path and use normally.

## 🛠️ Building from source

This project uses CMake to create cross-platform builds. As a pre-requisite, ensure that CMake and a recent C99 compiler is installed.

### Generate build files

For a debug build, run

```bash
cmake -S . -B debug
```

Change `debug` to `release` for a release build. Honestly, the folder name doesn't matter in this step. The config value matters in the next step.

### Start the build

For debug:

```bash
cmake --build debug
```

For release:

```bash
cmake --build release --config Release
```

### Run the executables

Two executables are generated in the `debug` (or `release`) folder. They are:

1. **run-tests(.exe)** - Run the unit tests for the project. All should pass. If anything fails, create an issue on Github.
2. **bump(.exe)** - The usable executable. Copy it to a folder in your path if you want it to be accessible from everywhere. Use as instructed above.

## 🙏 Precursors and acknowledgements

### CMake Basic Template

The base template is based off of my [starter CMake project](https://github.com/hungrybluedev/CMake-Basic-C-Template/). Almost all the material is derived from [An Introduction to Modern CMake](https://gitlab.com/CLIUtils/modern-cmake) by Henry Schreiner and others.

### Cross-platform simultaneous release template

The cross-platform release functionality is possible due to the Github Actions infrastructure. The basic template is available [here](https://github.com/hungrybluedev/release-test/). Specific people I want to thank are lukka and Simran-B. Visit the [original project](https://github.com/hungrybluedev/release-test/) to learn more about their contributions.

### Code Review

I want to thank G. Sliepen for an extremely helpful [code review](https://codereview.stackexchange.com/questions/251090/command-line-utility-to-bump-semantic-version-numbers-in-an-input-file). The code posted for review corresponds to the version tagged [v0.1.0](https://github.com/hungrybluedev/bump/releases/tag/v0.1.0).

## :handshake: Support

If you think my work has helped you, please consider supporting me on Ko-fi or Patreon.

[<img style="height: 36px;" height="36" src="https://raw.githubusercontent.com/hungrybluedev/hungrybluedev/master/kofi.webp">](https://ko-fi.com/hungrybluedev)
[<img style="height: 36px;" height="36"  src="https://raw.githubusercontent.com/hungrybluedev/hungrybluedev/master/patreon.webp">](https://www.patreon.com/hungrybluedev)
