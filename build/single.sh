#!/bin/bash

BUILD_DIR="."

cd "$BUILD_DIR" || {
    echo "error : build doesn't exist"
    exit 1
}

rm -f *.data
echo "已删除build目录下的所有.data文件"

make -j4
./bookstore < test.in > test.out