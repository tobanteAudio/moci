#!/usr/bin/env sh

python scripts/run-clang-tidy.py -header-filter="moci" -fix -p build_Debug moci