#!/bin/bash

mkdir -p "lib/native/linux/8/x64/"
mkdir -p "lib/native/linux/9/x64/"
mkdir -p "lib/native/linux/10/x64/"
mkdir -p "lib/native/linux/11/x64/"

mkdir -p "lib/native/linux/8/ia32/"
mkdir -p "lib/native/linux/9/ia32/"
mkdir -p "lib/native/linux/10/ia32/"
mkdir -p "lib/native/linux/11/ia32/"

node-gyp clean configure build --arch=x64 --target=v8.14.0
mv "build/Release/watchdoger.node" "lib/native/linux/8/x64/"
node-gyp clean configure build --arch=ia32 --target=v8.14.0
mv "build/Release/watchdoger.node" "lib/native/linux/8/ia32/"

node-gyp clean configure build --arch=x64 --target=v9.11.2
mv "build/Release/watchdoger.node" "lib/native/linux/9/x64/"
node-gyp clean configure build --arch=ia32 --target=v9.11.2
mv "build/Release/watchdoger.node" "lib/native/linux/9/ia32/"

node-gyp clean configure build --arch=x64 --build_v8_with_gn=false --target=v10.14.2
mv "build/Release/watchdoger.node" "lib/native/linux/10/x64/"
node-gyp clean configure build --arch=ia32 --build_v8_with_gn=false --target=v10.14.2
mv "build/Release/watchdoger.node" "lib/native/linux/10/ia32/"

node-gyp clean configure build --arch=x64 --build_v8_with_gn=false --target=v11.4.0
mv "build/Release/watchdoger.node" "lib/native/linux/11/x64/"
node-gyp clean configure build --arch=ia32 --build_v8_with_gn=false --target=v11.4.0
mv "build/Release/watchdoger.node" "lib/native/linux/11/ia32/"

rm -r build