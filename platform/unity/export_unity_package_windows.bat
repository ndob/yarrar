set UNITY_EXECUTABLE=E:\applications\Unity3d\Editor\Unity.exe
set PROJECT_DIR="%cd%\yarrar"

%UNITY_EXECUTABLE% -batchmode -projectPath %PROJECT_DIR% -exportPackage Assets\yarrar\Plugins Assets\yarrar\Prefabs Assets\yarrar\Scripts yarrar.unitypackage -quit
