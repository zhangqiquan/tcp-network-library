aux_source_directory(../../src SOURCES)
aux_source_directory(../../src/http HTTPSOURCES)
message (STATUS "载入HTTP模块文件")
message (STATUS "${HTTPSOURCES}")
message (STATUS "载入HTTP模块文件结束")
project(zhanshop-websocket)
add_compile_options(-Wall -g -rdynamic)
add_executable(zhanshop-websocket websocket.cc ${SOURCES} ${HTTPSOURCES})

