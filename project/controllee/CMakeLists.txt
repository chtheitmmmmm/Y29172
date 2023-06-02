project(controller
        LANGUAGES C
        DESCRIPTION 主控端)

file(GLOB_RECURSE SRC ${PROJECT_PROJECT_DIR}/controllee/src/*.c)

add_executable(cle ${SRC} ${LIBRARY_SRC})

######################################################################
set(SERVER_IP_DEFAULT 127.0.0.1)        # 默认 ip 地址
set(SERVER_IP 10.177.77.177)     # 这里指定服务器 IPv4 地址
set(SERVER_PORT_DEFAULT 29001)          # 默认端口
set(SERVER_PORT ${SERVER_PORT_DEFAULT}) # 这里指定服务器端口
######################################################################

add_compile_definitions(CONFIG_DEFAULT_SERVER_IP="${SERVER_IP}")
add_compile_definitions(CONFIG_DEFAULT_SERVER_PORT=${SERVER_PORT})
target_link_libraries(cle PRIVATE hv_static event ssl)