#!/bin/bash

if [ "$1" == "fresh" ]; then
    rm -rf ./build
fi

cmake . -B build
if [ $? -ne 0 ]; then
    exit 1
fi

cd build || { exit 1; }

make
if [ $? -ne 0 ]; then
    exit 1
fi

cd ..

./build/main

