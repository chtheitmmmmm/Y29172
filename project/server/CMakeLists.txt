project(server
        LANGUAGES C
        DESCRIPTION 主控端)

file(GLOB_RECURSE SRC ${PROJECT_PROJECT_DIR}/server/src/*.c)

add_executable(server ${SRC} ${LIBRARY_SRC})
target_link_libraries(server PRIVATE hv_static event ssl) # library linkage
