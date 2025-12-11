cmake_minimum_required(VERSION 3.10)

# 项目名称（可以自己改）
project(BOOKSTORE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build" FORCE)
endif()

# 设置C++标准（根据你的需求选，比如C++17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录（对应你的include文件夹）
include_directories(include)

# 收集src目录下的所有cpp文件（也可以手动列出来）
file(GLOB SOURCES "src/*.cpp")

# 生成可执行文件（bookstore是生成的程序名，SOURCES是所有源文件）
add_executable(code ${SOURCES})