@echo off
echo "Submodule download"
git submodule init
git submodule update

@REM echo "GLFW build"
@REM pushd "external/glfw"
@REM cmake -DBUILD_SHARED_LIBS=OFF -B build -S .
@REM cmake --build ./build --config Release
@REM popd
@REM
@REM echo "OpenAL build"
@REM pushd "external/openal"
@REM cmake -B build -S .
@REM cmake --build ./build --config Release
@REM popd
@REM
@REM echo "Copy openal dll"
@REM xcopy .\external\openal\build\Release\OpenAL32.dll .\ /Y