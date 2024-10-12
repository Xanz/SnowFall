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

echo "zlib build"
pushd "external/zlib"
cmake -B build -S . -A Win32 -DBUILD_SHARED_LIBS=OFF
cmake --build ./build --config Release
popd

echo "JPEG-6 build"
pushd "external/jpeg-6"
cmake -B build -S . -A Win32
cmake --build ./build --config Release
popd


echo "OpenAL build"
pushd "external/openal"
cmake -B build -S . -A Win32
cmake --build ./build --config Release
popd

echo "Copy openal dll"
xcopy .\external\openal\build\Release\OpenAL32.dll .\build\Debug\ /Y
xcopy .\external\openal\build\Release\OpenAL32.dll .\build\Release\ /Y