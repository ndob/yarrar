# yarrar
[![Build Status](https://travis-ci.com/ndob/yarrar.svg?token=VufYWjmk7wxVPM2557Mq)](https://travis-ci.com/ndob/yarrar)

## Building

### Android
libyarrar:
```shell
export ANDROID_NDK_ROOT=[path-to-ndk]
mkdir android_build
cd android_build
cmake -DCMAKE_TOOLCHAIN_FILE=../external/android-cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" ..
make
```

yarrar.aar (Android Java library):
```shell
cd platform/android
./build_aar.sh
```

Example-app:
1. First build AAR (gets automatically copied to example/android/app/libs)
2. Build with gradle or Android studio
