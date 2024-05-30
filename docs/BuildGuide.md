# Build Instructions

Currently the engine only supports building for Windows. So if you are attempting to build with linux please come back later or you can attempt to use the existing scons build system that was present in the original GPL release.(I provide no support for the SCons system)

## Dependencies

Before you start with the build process make sure you have the following software installed:

- MSVC compiler
- CMake
- Code editor
- Git
- Doom 3 (2004)

## Build

1. Clone down the repository.
2. Run the “BuildDep.bat” script.
3. Create an extracted version of the Doom 3 base folder. 
4. Copy the base folder into the root repository folder. (Do not replace the files)
5. Run the “ConvertSounds.bat” script.
6. Build the repository using CMake GUI or command line.
7. The “Doom3.exe” file will be exported to the top level.
8. You can either run this from a debugger or simply launch exectuable.
9. Doom 3 should launch.