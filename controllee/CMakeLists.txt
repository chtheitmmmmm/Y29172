project(controllee
        LANGUAGES C
        DESCRIPTION 受控端)

file(GLOB_RECURSE SRC ${PROJECT_DIR}/controllee/src/*.c)

add_executable(controllee ${SRC})