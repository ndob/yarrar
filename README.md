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

