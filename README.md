# tobanteAudio - moci

## Status

|                                                                 License                                                                 |                                                       Linux                                                       |                                                           Coverage                                                           |                                    Issues                                    |
| :-------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------: |
| [![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://github.com/tobanteAudio/moci/blob/master/LICENSE) | [![CircleCi](https://circleci.com/gh/tobanteAudio/moci.svg?style=svg)](https://circleci.com/gh/tobanteAudio/moci) | [![codecov](https://codecov.io/gh/tobanteAudio/moci/branch/master/graph/badge.svg)](https://codecov.io/gh/tobanteAudio/moci) | ![GitHub issues](https://img.shields.io/github/issues/tobanteAudio/moci.svg) |

## Quick Start

- C++17
- CMake > 3.13
- Ninja Build

Clone with submodules:

```sh
git clone --recursive https://github.com/tobanteAudio/moci.git
```

```sh
CONFIG=Debug make config build
CONFIG=Debug make config-gles build # Link against OpenGLES 2.0
```

### clang-tidy

Two options. Somehow not all headers are found in either option:

```sh
CONFIG=Debug make tidy
CONFIG=Debug make tidy-fix
./scripts/moci-clang-tidy.sh
```
