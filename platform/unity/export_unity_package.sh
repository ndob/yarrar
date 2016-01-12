#!/bin/bash
CWD=`pwd`
UNITY_PATH=/opt/Unity/Editor/Unity

echo Compiling Android-native library...

mkdir ../../unity_android_build
cd ../../unity_android_build
cmake -DUNITY_PLUGIN=ON -DCMAKE_TOOLCHAIN_FILE=../external/android-cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" -DANDROID_NATIVE_API_LEVEL=9 ..
make
if [ ! $? -eq 0 ] 
then
    echo Error compiling native Android-library. Exiting.
    exit 1
fi

cd ..

echo Compiling Linux-native library...

mkdir unity_linux_build
cd unity_linux_build
cmake -DUNITY_PLUGIN=ON ..
make
if [ ! $? -eq 0 ] 
then
    echo Error compiling native Linux-library. Exiting.
    exit 1
fi

cd $CWD
mkdir -p $CWD/yarrar/Assets/yarrar/Plugins/x86_64
cp $CWD/../../unity_linux_build/build/lib/libyarrar.so $CWD/yarrar/Assets/yarrar/Plugins/x86_64/libyarrar.so

mkdir -p $CWD/yarrar/Assets/yarrar/Plugins/Android
cp $CWD/../../unity_android_build/build/lib/libyarrar.so $CWD/yarrar/Assets/yarrar/Plugins/Android/libyarrar.so

$UNITY_PATH -batchmode -projectPath $CWD/yarrar -exportPackage "Assets/yarrar/Plugins" "Assets/yarrar/Prefabs" "Assets/yarrar/Scripts" yarrar.unitypackage -quit
