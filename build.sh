#!/bin/bash

if ! which apt; then
  PM=apt
elif ! which yum; then
  PM=yum
else
  echo "No package manager found."
fi

if ! which cmake; then
  echo "Cmake cannot find, install"
  sudo ${PM} install cmake
  echo "\x1b[33mCannot install cmake. Abort.\x1b[0m"
else
  echo "检测到CMake版本 $(cmake --version | cut -d ' ' -f3)"
fi

sudo ${PM} install libevent-dev

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=make -DCMAKE_C_COMPILER=gcc -S . -B ./cmake-build-debug