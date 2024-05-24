@echo off

echo "Deploy base folder"
xcopy "C:\Program Files (x86)\Steam\steamapps\common\DOOM 3 BFG Edition\base" "build/Debug/base/" /E /H /C /I
xcopy "C:\Program Files (x86)\Steam\steamapps\common\DOOM 3 BFG Edition\base" "build/Release/base/" /E /H /C /I

echo "Deploy shaders .EXE locations"
xcopy ".\shaders" "build/Debug/base/" /E /H /C /I
xcopy ".\shaders" "build/Release/base/" /E /H /C /I

@REM echo "Deploy library dynamic libs"
xcopy ".\build\external\openal\Debug\OpenAL32.dll" ".\build\Debug\" /H /C /Y
xcopy ".\build\external\openal\Release\OpenAL32.dll" ".\build\Release\" /H /C /Y