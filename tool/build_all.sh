#!/bin/bash
CWD=`pwd`
NUMBER_OF_CORES=`nproc`
YARRAR_ROOT_PATH=../../..

function check_error {
    if [ ! $? -eq 0 ] 
    then
        echo Error compiling [$1]. Exiting.
        exit 1
    fi
}

rm -rf buildtemp

mkdir -p buildtemp/android
mkdir -p buildtemp/linux
mkdir -p buildtemp/unity

cd $CWD/buildtemp/android
cmake -DCMAKE_TOOLCHAIN_FILE=$YARRAR_ROOT_PATH/external/android-cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" -DANDROID_NATIVE_API_LEVEL=9 $YARRAR_ROOT_PATH
make -j $NUMBER_OF_CORES && make test ARGS="-V"
check_error android

cd $CWD/buildtemp/linux
cmake $YARRAR_ROOT_PATH
make -j $NUMBER_OF_CORES && make test ARGS="-V"
check_error linux

cd $CWD/buildtemp/unity
cmake -DUNITY_PLUGIN=ON $YARRAR_ROOT_PATH
make -j $NUMBER_OF_CORES
check_error unity
