@echo off
echo "Submodule download"
git submodule init
git submodule update

echo "GLFW build"
pushd "external/glfw"
cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -A x64 -B build -S .
cmake --build ./build --config Release
popd

echo "zlib build"
pushd "external/zlib"
cmake -B build -S . -A x64 -DBUILD_SHARED_LIBS=OFF
cmake --build ./build --config Release
popd

echo "JPEG-6 build"
pushd "external/jpeg-6"
cmake -B build -S . -A x64
cmake --build ./build --config Release
popd


echo "OpenAL build"
pushd "external/openal"
cmake -B build -S . -A x64
cmake --build ./build --config Release
popd

echo "Copy openal dll"
xcopy .\external\openal\build\Release\OpenAL32.dll .\build\Debug\ /Y
xcopy .\external\openal\build\Release\OpenAL32.dll .\build\Release\ /Y

echo "Copying game data"
xcopy /E "C:\Program Files (x86)\Steam\steamapps\common\DOOM 3 BFG Edition\base\" .\build\Debug\base\ /Y
xcopy /E "C:\Program Files (x86)\Steam\steamapps\common\DOOM 3 BFG Edition\base\" .\build\Release\base\ /Y

echo "Deploying shader files"
xcopy /E .\renderprogs\ .\build\Debug\base\renderprogs\ /Y
xcopy /E .\renderprogs\ .\build\Release\base\renderprogs\ /Y
