#!/usr/bin/env sh

cd build_Debug && ../scripts/run-clang-tidy.py ../moci/moci -p . -fix -header-filter="moci/moci/.*"
cd build_Debug && ../scripts/run-clang-tidy.py ../sandbox -p . -fix -header-filter="moci/moci/.*"
cd build_Debug && ../scripts/run-clang-tidy.py ../sandbox3D -p . -fix -header-filter="moci/moci/.*"
