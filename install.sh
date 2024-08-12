#!/bin/bash

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