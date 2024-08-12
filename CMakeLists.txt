cmake_minimum_required(VERSION 3.10)
project(ChatServer)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 生成可执行文件目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 头文件路径
include_directories(${PROJECT_SOURCE_DIR}/thirdpart)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/model)
include_directories(${PROJECT_SOURCE_DIR}/include/server/database)
include_directories(${PROJECT_SOURCE_DIR}/include/client)

# 添加src子目录
add_subdirectory(src)