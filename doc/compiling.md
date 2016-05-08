# Compiling libyarrar

Yarrar uses cmake compilation system. Minimum version for Linux is 3.2 and for Windows 3.4. 

Linux builds have been tested with Ubuntu 14.04 LTS (GCC 4.8 and clang 3.4) and Windows builds with Windows 7 (Visual Studio Community Edition 2015).

## Linux (libyarrar.so)
```
mkdir build
cd build
cmake ..
make
```
This compiles both libyarrar and desktop_webcam-example.

## Windows (yarrar.dll)
Create Visual Studio solution with cmake-gui or:
```
cmake ..
```
Open in Visual Studio and press build.

This compiles both libyarrar and desktop_webcam-example.

## Android (yarrar.aar and libyarrar.so)

Android compilation is tested and supported on **Linux only**. 

Android AAR-library (Java-library) can be built using the helper script in *platform/android*. This automates building native library and bundling it with necessary Java-code. Final .aar-file will be placed in *platform/yarrar/build/outputs/aar/yarrar-release.aar*

```shell
cd platform/android
./build_aar.sh
```

Use cmake, if you wish to only build .so-native library for Android. This produces an armv7-binary. Other architectures can be built by adjusting ANDROID_ABI-setting.

```shell
export ANDROID_NDK_ROOT=[path-to-ndk]
mkdir android_build
cd android_build
cmake -DCMAKE_TOOLCHAIN_FILE=../external/android-cmake/android.toolchain.cmake -DANDROID_NDK=$ANDROID_NDK_ROOT -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" -DANDROID_NATIVE_API_LEVEL=9 ..
make
```

## iOS (Yarrar.framework)

iOS compilation is tested and supported from **Xcode only** (CMake not currently supported).

Package can be compiled for **armv7 and armv7s only**. This is a limitation of prebuilt OpenCV 3.0, so building it manually should get around this issue. In that case edit *platform/ios/Xcode/ios-build-framework.sh* to support arm64.

1. Download and extract OpenCV 3.0 for iOS (http://opencv.org/downloads.html) to *platform/ios/Xcode/Yarrar*-directory.
2. Open and build *platform/ios/Xcode/Yarrar.xcodeproj*
3. Built Yarrar.framework-bundle will appear in *platform/ios/Xcode/output*

## Unity

**Linux+Android-plugin (compilation on Linux):**

```shell
cd platform/unity
./export_unity_package.sh
```
This compiles Linux and Android .so-libraries and packages them with necessary C#-code to a .unitypackage-distribution. Final .unitypackage-file will be in *platform/unity/yarrar*.

**Windows-plugin (compilation on Windows):**

Compile Windows .dll-library (see instructions above) and place yarrar.dll and opencv_world310.dll to *platform/unity/yarrar/Assets/yarrar/Plugins*

Then run:
```shell
cd platform/unity
export_unity_package_windows.bat
```
This packages Windows .dll-library and necessary C#-code to a .unitypackage-distribution. Final .unitypackage-file will be in *platform/unity/yarrar*.
