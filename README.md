# tobanteAudio - moci

General purpose application library. Currently supports desktop & mobil OpenGL. Runs on devices which only support OpenGLES 2.0.

## Status

- [GitHub Repository](https::/github.com/tobanteAudio/moci)
- [API Reference](https://tobanteaudio.github.io/moci/classes.html)

|                                                                 License                                                                 |                                                         macOS                                                         |                                                                 Windows                                                                 |                                                       Linux                                                       |                                                           Coverage                                                           |                                    Issues                                    |
| :-------------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------: | :---------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------: |
| [![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://github.com/tobanteAudio/moci/blob/master/LICENSE) | [![Build Status](https://travis-ci.org/tobanteAudio/moci.svg?branch=master)](https://travis-ci.org/tobanteAudio/moci) | [![AppVeyor Build status](https://img.shields.io/appveyor/ci/tobanteAudio/moci.svg)](https://ci.appveyor.com/project/tobanteAudio/moci) | [![CircleCi](https://circleci.com/gh/tobanteAudio/moci.svg?style=svg)](https://circleci.com/gh/tobanteAudio/moci) | [![codecov](https://codecov.io/gh/tobanteAudio/moci/branch/master/graph/badge.svg)](https://codecov.io/gh/tobanteAudio/moci) | ![GitHub issues](https://img.shields.io/github/issues/tobanteAudio/moci.svg) |

## Quick Start

- C++17
- CMake > 3.13
- Ninja Build

Clone with submodules:

```sh
git clone --recursive https://github.com/tobanteAudio/moci.git
```

### macOS & Linux

Running the `Makefile` builds the project with your default compiler & ninja.

```sh
# Link against desktop OpenGL.
CONFIG=Debug make
# Same as
CONFIG=Debug make config build test
# Link against OpenGLES 2.0. This also excludes glew from being build.
CONFIG=Debug make config-gles build test
```

### Windows (Visual Studio)

Running the `Makefile` builds the project Visual Studio.

```sh
# Link against desktop OpenGL.
CONFIG=Debug make config-vs build test
# or
CONFIG=Release make config-vs build test
```

## Features

- Application
  - Cross platform window creation. Currently based on `GLFW`.
- Rendering
  - `ImGui` integration
  - 2D batch renderer
  - Custom shaders, buffers & textures
  - Fonts
- Geometry
  - Basic shapes & transformations, using `glm`.
- Network
  - Cross platform UDP socket listen & write
- System
  - Query CPU & OS information
- Debug
  - Create profiles readable by `Chrome` tracing.

## Long Term Goals

- Application
  - Native windowing APIs.
- Audio
  - Native sound APIs.
  - Audio Plugins. (VST3, AudioUnit)
- Rendering
  - Metal, DirectX & Vulkan. (Priority as listed)

## Development

### Dependencies (submodules)

- [assimp: 3D Model loader](https://github.com/assimp/assimp)
- [Catch2: Unit tests](https://github.com/catchorg/Catch2)
- [freetype: Font loader](https://www.freetype.org/)
- [glew: OpenGL function loader](http://glew.sourceforge.net/)
- [glfw: Cross platform window library](https://www.glfw.org/)
- [glm: Math library](https://glm.g-truc.net/0.9.9/index.html)
- [GSL: C++ guideline support library](https://github.com/microsoft/GSL)
- [imgui: GUI library](https://github.com/ocornut/imgui)
- [spdlog: Logging](https://github.com/gabime/spdlog)
- [stb: Image file loader](https://github.com/nothings/stb)

### Dependencies Linux

Look at the `circleci` config in [`.circleci/config.yml`](https://github.com/tobanteAudio/moci/blob/master/.circleci/config.yml) for Ubuntu packages. Otherwise just try building the project. CMake will tell you whats missing.

### clang-tidy

Two options. Somehow not all headers are found in either option:

```sh
CONFIG=Debug make tidy
CONFIG=Debug make tidy-fix
./scripts/moci-clang-tidy.sh
```
