# libyarrar [![Build Status](https://travis-ci.com/ndob/yarrar.svg?token=VufYWjmk7wxVPM2557Mq)](https://travis-ci.com/ndob/yarrar)

## Overview
A cross-platform Augmented Reality library written in C++11. 

Strongly WIP, so don't expect to use this for anything real at the moment. :)

### Current features
* Supported platforms:
   * Linux
   * Windows
   * Android
   * Unity3D-plugin (Windows, Linux and Android). Unity 5.3 required.
* Extendable pipeline architechture (see [Architechture](#architechture))
* Marker tracking: multiple markers
* OpenGL-rendering: only simple 3D-shapes supported for the moment

### Roadmap
* Support for sensor data (accelerometer, gyroscope)
* Sensor fusion pipeline-stage
* Support for more trackers
    * SLAM
    * Edge-based
    * Face
* Platform support for iOS and OSX (native and Unity3D-plugin)
* Improved marker detection / different marker data-fields
* Support for a proper rendering-library (Ogre3D or OpenSceneGraph)

### Architechture

Yarrar is modeled as a pipeline, which consists of four stages. Each stage has one or more stage-handlers, which process the data from previous stage and pass it forward to next stage. Pipeline configuration is done with JSON-based configuration file (see [Pipeline Configuration](doc/pipeline-configuration.md) for details). Pipeline stages are:

1. Dataprovider: Sensor data handling (ie. RGB-camera, depth-camera, accelerometer, gyroscope)
2. Tracker: Pose tracking (ie. using markers, SLAM)
3. SensorFusion: Pose accuracy improvement
4. Render: Rendering the 3D-models on top of camera image

![Image of Pipeline](http://yuml.me/diagram/scruffy/class/%5BDataprovider%5D-%3E%5BTracker%5D,%5BTracker%5D-%3E%5BSensorFusion%5D,%5BSensorFusion%5D-%3E%5BRender%5D)

**Limitations in current implementation**:
* Only one dataprovider with *RGB_CAMERA_FLAG* set is supported
* SensorFusion-stage is not implemented

## Dependencies
Yarrar depends heavily on C++11-features, so a fairly recent compiler is needed.

Library dependencies per compilation target:

| Platform          | OpenCV | json11 | GLEW | GLFW |
| --------          | ------ | ------ | ---- | ---- |
| Linux             | X      | X      | X    | X    |
| Windows           | X      | X      | X    | X    |
| Android           | X      | X      |      |      |
| Unity3D (Windows) | X      | X      |      |      |
| Unity3D (Android) | X      | X      |      |      |

## Compiling and running tests

See [Compiling](doc/compiling.md) for detailed instructions

### Quickstart

Compile and run tests on Linux:
```
mkdir build
cd build
cmake ..
make
make test ARGS="-V"
```

### Compiling examples

#### Android

1. Build AAR (see [Compiling](doc/compiling.md)). 
2. If you built it with build_aar.sh-script, .aar gets automatically copied to *example/android/app/libs*.
3. Open and build *example/android* with gradle or Android studio.

#### Windows/Linux

1. See [Quickstart](#quickstart). It will also compile *example/desktop_webcam*-example by default.

#### Unity

1. Compile yarrar.unitypackage (see [Compiling](doc/compiling.md)).
2. Open up *example/unity* in Unity Editor. Basic structure should be set up.

##### Known issues (Unity)
* When importing yarrar.unitypackage to Unity on Linux following error might appear:

> Multiple plugins with the same name 'libyarrar' (found at 'Assets/yarrar/Plugins/x86_64/libyarrar.so' and 
> 'Assets/yarrar/Plugins/Android/libyarrar.so'). That means one or more plugins are set to be compatible with Editor. Only one
> plugin at the time can be used by Editor.*

Fix this by deleting *yarryar/Plugins/x86_64* if you wish to deploy to Android or *yarryar/Plugins/Android* if you wish to deploy to Linux.

* Deploying to Android might fail, if you don't set correct architecture for .so-library. This can be done by selecting libyarrar.so-file from *Project*-view and setting CPU to **ARMv7** from *Inspector*-window.
