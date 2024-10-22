cmake_minimum_required(VERSION 3.13.0)
project(SnowFall LANGUAGES C CXX)

file(GLOB D3GAME src/game/*.cpp src/game/*.h src/game/Game.def)
file(GLOB D3GAME_AI src/game/ai/*.cpp src/game/ai/*.h)
file(GLOB D3GAME_ANIM src/game/anim/*.cpp src/game/anim/*.h)
file(GLOB D3GAME_GAMESYS src/game/gamesys/*.cpp src/game/gamesys/*.h)

# Removes callbacks. This is manually included in the right place.
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
file(GLOB_RECURSE SFRenderer src/sfRenderer/*.cpp src/sfRenderer/*.h)

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
        ${SFRenderer}
)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
add_compile_definitions(GLEW_STATIC)

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/idlib PUBLIC external/openal/include external/jpeg-6/include external/zlib external/zlib/build external/glew/include external/glfw/include)

link_directories(external/openal/build/Release external/jpeg-6/build/Release external/zlib/build/Release external/glew/lib external/glfw/build/src/Release)

set(LIBS xinput dbghelp dinput8 dxguid glu32 iphlpapi odbc32 odbccp32 opengl32 winmm ws2_32 zlibstatic libjpeg OpenAL32 glew32s glfw3)

add_executable(${PROJECT_NAME} ${D3Source})

target_precompile_headers(${PROJECT_NAME} PRIVATE src/idLib/precompiled.h)

target_link_libraries(${PROJECT_NAME} ${LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)
