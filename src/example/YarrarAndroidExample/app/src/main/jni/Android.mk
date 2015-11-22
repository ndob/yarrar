# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
EXTERNAL_OPEN_CV_INCLUDE_PATH := $(LOCAL_PATH)/../../../../../../../external/OpenCV-android-sdk/sdk/native/jni/include/
EXTERNAL_OPEN_CV_LINKER_PATH := ../../../../../../../external/OpenCV-android-sdk/sdk/native/libs/
YARRAR_INCLUDE_PATH := $(LOCAL_PATH)/../../../../../../
YARRAR_SOURCE_PATH := ../../../../../../yarrar/

$(warning $(EXTERNAL_OPEN_CV_INCLUDE_PATH))
$(warning $(YARRAR_INCLUDE_PATH))

include $(CLEAR_VARS)

LOCAL_MODULE    := opencv_core
LOCAL_SRC_FILES := $(EXTERNAL_OPEN_CV_LINKER_PATH)$(TARGET_ARCH_ABI)/libopencv_java3.so

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := yarrar_glue
LOCAL_SRC_FILES :=  yarrar_glue.cpp \
                    $(YARRAR_SOURCE_PATH)/Pipeline.cpp \
                    $(YARRAR_SOURCE_PATH)/dataprovider/StaticImageDataProvider.cpp \
                    $(YARRAR_SOURCE_PATH)/detector/MarkerDetector.cpp

LOCAL_C_INCLUDES := $(EXTERNAL_OPEN_CV_INCLUDE_PATH) $(YARRAR_INCLUDE_PATH) $(YARRAR_INCLUDE_PATH)/yarrar
LOCAL_STATIC_LIBRARIES := opencv_core
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
