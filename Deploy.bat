@echo off

echo "Deploy base folder"
xcopy "C:\Program Files (x86)\Steam\steamapps\common\DOOM 3 BFG Edition\base" "build/Debug/base/" /E /H /C /I
xcopy "C:\Program Files (x86)\Steam\steamapps\common\DOOM 3 BFG Edition\base" "build/Release/base/" /E /H /C /I

echo "Deploy shaders .EXE locations"
xcopy ".\shaders" "build/Debug/base/" /E /H /C /I
xcopy ".\shaders" "build/Release/base/" /E /H /C /I

echo "Deploy library dynamic libs"
xcopy ".\external\glfw\libs\glfw3.dll" ".\build\Debug\" /H /C /Y
xcopy ".\external\OpenAL\build\Debug\OpenAL32.dll" ".\build\Debug\" /H /C /Y

xcopy ".\external\glfw\libs\glfw3.dll" ".\build\Release\" /H /C /Y
xcopy ".\external\OpenAL\build\Debug\OpenAL32.dll" ".\build\Release\" /H /C /Y