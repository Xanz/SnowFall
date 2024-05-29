@echo off
echo "Submodule download"
git submodule init
git submodule update

echo "GLFW build"
pushd "external/glfw"
cmake -DBUILD_SHARED_LIBS=OFF -A Win32 -B build -S .
cmake --build ./build --config Release
popd

echo "OpenAL build"
pushd "external/openal"
cmake -A Win32 -B build -S .
cmake --build ./build --config Release
popd

echo "Copy openal dll"
xcopy .\external\openal\build\Release\OpenAL32.dll .\ /Y

