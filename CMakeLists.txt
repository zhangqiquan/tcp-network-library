CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
project(zhanshop)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE -Os)

set(CMAKE_C_STANDARD 23)
# -Wall 生成所有警告信息 关闭-g等调试选项 文件要小的倍以上
add_compile_options(-Wall -g -rdynamic)
#include_directories(${PROJECT_SOURCE_DIR}/muduoXX/include)
add_subdirectory(examples)
#aux_source_directory(./src SOURCES)
add_executable(zhanshop index.cc)

target_link_libraries(zhanshop -lpthread)