#!/bin/bash

# 创建 thirdpart 目录
mkdir -p thirdpart
# 下载 json 库到 thirdpart
wget https://raw.gitmirror.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -O thirdpart/json.hpp
if [ $? -ne 0 ]; then
    echo "Failed to download json.hpp"
    exit 1
fi

# 创建并进入 build 目录
mkdir -p build
cd build || { echo "Failed to enter build directory"; exit 1; }

# 运行 cmake
if ! cmake ..; then
    echo "CMake configuration failed"
    exit 1
fi

# 编译项目
if ! make; then
    echo "Build failed"
    exit 1
fi

echo "Build completed successfully"