# yarrar
[![Build Status](https://travis-ci.com/ndob/yarrar.svg?token=VufYWjmk7wxVPM2557Mq)](https://travis-ci.com/ndob/yarrar)

## Building

### Android
libyarrar:
```shell
export ANDROID_NDK_ROOT=[path-to-ndk]
mkdir android_build
cd android_build
cmake -DCMAKE_TOOLCHAIN_FILE=../external/android-cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" -DANDROID_NATIVE_API_LEVEL=9 ..
make
```

yarrar.aar (Android Java library):
```shell
cd platform/android
./build_aar.sh
```

Example-app:
* First build AAR (see previous instructions). It gets automatically copied to example/android/app/libs.
* Open and build example/android with gradle or Android studio

### Unity (currently only works in windows)
cmake .. -DUNITY_PLUGIN=ON

### Windows
* Create Visual Studio solution with:
```
cmake ..
```

### Linux
```
mkdir build
cd build
cmake ..
make
```

## Running tests
```
mkdir build
cd build
cmake ..
make
make test ARGS="-V"
```
