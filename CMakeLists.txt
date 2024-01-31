cmake_minimum_required(VERSION 3.12)

project(TicTacToe)  # 项目名称

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)

# 设置源文件目录
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(BUILD_DIR, ${CMAKE_SOURCE_DIR}/build)

# 设置头文件目录
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# 设置库文件目录
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

# 包含头文件目录
include_directories(${INCLUDE_DIR})

file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")
# 添加可执行文件，将需要的源文件和库文件链接到一起
add_executable(tictactoe ${SOURCES})

# 添加到Build目录下的可执行文件
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${BUILD_DIR})

# 查找并包含需要的库文件
# find_library(YOUR_LIBRARY_NAME REQUIRED PATHS ${LIB_DIR})

# 链接需要的库文件
# target_link_libraries(YourExecutableName ${YOUR_LIBRARY_NAME})
