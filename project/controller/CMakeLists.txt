project(controller
        LANGUAGES C
        DESCRIPTION 主控端)

file(GLOB_RECURSE SRC ${PROJECT_PROJECT_DIR}/controller/src/*.c)

add_executable(clr ${SRC} ${LIBRARY_SRC})
target_link_libraries(clr PRIVATE hv_static event)