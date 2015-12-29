function(copy_dll_dependencies target_project)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/glew/bin/Release/x64/*.dll" GLEW_DLL_FILES)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/glfw/lib-vc2015/*.dll" GLFW_DLL_FILES)
    file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/external/opencv/build/x64/vc14/bin" OPENCV_DLL_FILES)
    file(TO_NATIVE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>" MSVC_BINARY_OUTPUT_PATH)

    add_custom_command(TARGET ${target_project} POST_BUILD
        COMMAND copy "${GLEW_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
    add_custom_command(TARGET ${target_project} POST_BUILD
        COMMAND copy "${GLFW_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
    add_custom_command(TARGET ${target_project} POST_BUILD
        COMMAND copy "${OPENCV_DLL_FILES}" "${MSVC_BINARY_OUTPUT_PATH}")
endfunction(copy_dll_dependencies)
