file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/cmake/windows/pullDependencies.ps1" PULL_DEPENDENCIES_COMMAND_PATH)

execute_process(
    COMMAND PowerShell -NoProfile -ExecutionPolicy Bypass -Command "& '${PULL_DEPENDENCIES_COMMAND_PATH}'"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
