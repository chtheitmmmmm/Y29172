#!/bin/bash

# 假设你运行在较新版本的 Ubuntu

if ! which cmake; then
  echo "Cmake cannot find, install"
  sudo apt install cmake
  echo "\x1b[33mCannot install cmake. Abort.\x1b[0m"
else
  echo "检测到CMake版本 $(cmake --version | cut -d ' ' -f3)"
fi

sudo apt install libevent-dev openssl libssl-dev

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=make -DCMAKE_C_COMPILER=gcc -S . -B ./cmake-build-debug