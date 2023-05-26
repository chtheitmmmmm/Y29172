project(controller
        LANGUAGES C
        DESCRIPTION 主控端)

file(GLOB_RECURSE SRC ${PROJECT_DIR}/controller/src/*.c)

add_executable(controller ${SRC})