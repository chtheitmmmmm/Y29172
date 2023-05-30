cmake_minimum_required(VERSION 3.21)
project(Y29172 C)

set(CMAKE_C_STANDARD 11)
set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)

set(PROJECT_PROJECT_DIR ${PROJECT_DIR}/project)
set(PROJECT_LIBRARY_DIR ${PROJECT_DIR}/library)

# include the library root directory
include_directories(library)
include_directories(library/libhv/include) # libhv

set(LIB_ARGPARSE_SRC ${PROJECT_LIBRARY_DIR}/argparse/argparse.c)
set(LIB_SHA256_SRC ${PROJECT_LIBRARY_DIR}/SHA256/sha256.c)
set(LIBRARY_SRC ${LIB_ARGPARSE_SRC} ${LIB_SHA256_SRC} ${LIB_CTIMESTAMP_SRC})

link_directories(${PROJECT_LIBRARY_DIR}/libhv/build/lib/) # link libhv

include(${PROJECT_PROJECT_DIR}/controller/CMakeLists.txt)
include(${PROJECT_PROJECT_DIR}/controllee/CMakeLists.txt)
include(${PROJECT_PROJECT_DIR}/server/CMakeLists.txt)
