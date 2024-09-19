#!/bin/bash

# 如果 thirdpart 目录不存在，则创建
if [ ! -d "thirdpart" ]; then
    mkdir thirdpart
fi

# 如果 json.hpp 文件不存在，则下载
if [ ! -f "thirdpart/json.hpp" ]; then
    wget https://raw.gitmirror.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -O thirdpart/json.hpp
    if [ $? -ne 0 ]; then
        echo "Failed to download json.hpp"
        exit 1
    fi
else
    echo "json.hpp already exists, skipping download."
fi

# 创建并进入 build 目录
if [ ! -d "build" ]; then
    mkdir build
fi
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