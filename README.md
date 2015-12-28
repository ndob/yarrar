# yarrar
[![Build Status](https://travis-ci.com/ndob/yarrar.svg?token=VufYWjmk7wxVPM2557Mq)](https://travis-ci.com/ndob/yarrar)

## Building

### Android
```shell
export ANDROID_NDK_ROOT=[path-to-ndk]
mkdir android_build
cd android_build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/android/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" -DBUILD_SHARED_LIBS=ON ..
make
```
### Windows
Download:
http://netix.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0-win32.zip -> extract to external/glew
https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.bin.WIN64.zip -> extract to external/glfw
http://netassist.dl.sourceforge.net/project/opencvlibrary/opencv-win/3.1.0/opencv-3.1.0.exe -> extract to external/opencv


### Unity
cmake .. -DUNITY_PLUGIN=ON
