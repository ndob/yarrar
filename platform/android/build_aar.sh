#!/bin/bash
mkdir ../../android_build
cd ../../android_build
cmake -DCMAKE_TOOLCHAIN_FILE=../external/android-cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" ..
make
mkdir -p ../platform/android/yarrar/src/main/libs/armeabi-v7a/
cp build/lib/libyarrar.so ../platform/android/yarrar/src/main/libs/armeabi-v7a/
cd ../platform/android
# gradle should be version > 2.2
gradle wrapper
./gradlew assembleRelease
mkdir -p ../../example/android/app/libs/
cp yarrar/build/outputs/aar/yarrar-release.aar ../../example/android/app/libs/yarrar.aar
