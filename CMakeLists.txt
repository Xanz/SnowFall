cmake_minimum_required(VERSION 3.13.0)
project(SnowFall LANGUAGES C CXX)

file(GLOB D3GAME src/game/*.cpp src/game/*.h src/game/Game.def)
file(GLOB D3GAME_AI src/game/ai/*.cpp src/game/ai/*.h)
file(GLOB D3GAME_ANIM src/game/anim/*.cpp src/game/anim/*.h)
file(GLOB D3GAME_GAMESYS src/game/gamesys/*.cpp src/game/gamesys/*.h)
list(REMOVE_ITEM D3GAME_GAMESYS "${CMAKE_SOURCE_DIR}/src/game/gamesys/Callbacks.cpp")
file(GLOB D3GAME_MENUS src/game/menus/*.cpp src/game/menus/*.h)
file(GLOB D3GAME_PHYS src/game/physics/*.cpp src/game/physics/*.h)
file(GLOB D3GAME_SCRIPT src/game/script/*.cpp src/game/script/*.h)

file(GLOB_RECURSE AAS src/aas/*.cpp src/aas/*.h)
file(GLOB_RECURSE CM src/cm/*.cpp src/cm/*.h)
file(GLOB_RECURSE FRAMEWORK src/framework/*.cpp src/framework/*.h)
file(GLOB_RECURSE IDLIB src/idlib/*.cpp src/idlib/*.h)
file(GLOB_RECURSE RENDERER src/renderer/*.cpp src/renderer/*.h src/renderer/*.c)
file(GLOB_RECURSE SOUND src/sound/*.cpp src/sound/*.h)
file(GLOB_RECURSE SWF src/swf/*.cpp src/swf/*.h)
file(GLOB SYS src/sys/*.cpp src/sys/*.h)
file(GLOB SYSWIN src/sys/win32/*.cpp src/sys/win32/*.h)
file(GLOB_RECURSE UI src/ui/*.cpp src/ui/*.h)
file(GLOB_RECURSE JPEG6 external/jpeg-6/src/*.cpp)

set(D3Source
    ${D3GAME}
    ${D3GAME_AI}
    ${D3GAME_ANIM}
    ${D3GAME_GAMESYS}
    ${D3GAME_MENUS}
    ${D3GAME_PHYS}
    ${D3GAME_SCRIPT}
    ${FRAMEWORK}
    ${IDLIB}
    ${AAS}
    ${CM}
    ${RENDERER}
    ${SOUND}
    ${SWF}
    ${SYS}
    ${SYSWIN}
    ${UI}
    ${JPEG6}
    )

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the C standard (optional)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
    
    
## Get the current time at build time
#string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")
#
## Pass the build time as a preprocessor definition to the compiler
#add_definitions(-DBUILD_TIME="${BUILD_TIME}")

add_compile_definitions(__DOOM__)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
add_compile_definitions(_USE_32BIT_TIME_T)

add_subdirectory(external/zlib)

include_directories(${CMAKE_SOURCE_DIR}/src  ${CMAKE_SOURCE_DIR}/src/idlib PUBLIC external/openal/include external/jpeg-6/include external/zlib)
link_directories(external/openal/lib)

set(LIBS xinput dbghelp dinput8 dxguid glu32 iphlpapi odbc32 odbccp32 opengl32 winmm ws2_32 OpenAL32 zlibstatic)

add_executable(${PROJECT_NAME} WIN32 ${D3Source})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
target_precompile_headers(${PROJECT_NAME} PRIVATE src/idLib/precompiled.h)

target_link_libraries(${PROJECT_NAME} ${LIBS})

#add_custom_target(ForceRegenerate
#    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}
#    COMMENT "Forcing CMake regeneration"
#)

#add_dependencies(${PROJECT_NAME} ForceRegenerate)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
#)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
#    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
#)