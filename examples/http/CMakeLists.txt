aux_source_directory(../../src SOURCES)
aux_source_directory(../../src/http HTTPSOURCES)

project(zhanshop-http)
add_compile_options(-Wall -g -rdynamic)
add_executable(zhanshop-http http.cc ${SOURCES} ${HTTPSOURCES})

