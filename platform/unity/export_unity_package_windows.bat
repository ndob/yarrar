@echo off
set UNITY_EXECUTABLE=
set PROJECT_DIR="%cd%\yarrar"

if "%UNITY_EXECUTABLE%"=="" (
    echo ERROR: UNITY_EXECUTABLE not set. Fix it by editing this .bat-file and pointing the variable to Unity.exe.
    exit /b
)

%UNITY_EXECUTABLE% -batchmode -projectPath %PROJECT_DIR% -exportPackage Assets\yarrar\Plugins Assets\yarrar\Prefabs Assets\yarrar\Scripts yarrar.unitypackage -quit
