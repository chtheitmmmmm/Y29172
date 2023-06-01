project(controllee
        LANGUAGES C
        DESCRIPTION 受控端)

file(GLOB_RECURSE SRC ${PROJECT_PROJECT_DIR}/controllee/src/*.c)

add_executable(cle ${SRC} ${LIBRARY_SRC})
target_link_libraries(cle PRIVATE hv_static event ssl)