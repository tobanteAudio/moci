# tobanteAudio - moci

## Quick Start

```sh
CONFIG=Debug make config build
CONFIG=Debug make config-gles build # OpenGLES 2.0
```

### clang-tidy

Two options. Somehow not all headers are found in either option:

```sh
CONFIG=Debug make tidy
CONFIG=Debug make tidy-fix
./scripts/moci-clang-tidy.sh
```
