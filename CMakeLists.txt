cmake_minimum_required(VERSION 3.25)
project(Y29172 C)

set(CMAKE_C_STANDARD 11)
set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_DIR}/bin)

include(${PROJECT_DIR}/controller/CMakeLists.txt)
include(${PROJECT_DIR}/controllee/CMakeLists.txt)
