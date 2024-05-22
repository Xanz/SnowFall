cmake_minimum_required(VERSION 3.13.0)
project(Doom3 VERSION 1.4.3)

file(GLOB D3GAME src/d3xp/*.cpp src/d3xp/*.h src/d3xp/game.def)
file(GLOB D3GAME_AI src/d3xp/ai/*.cpp src/d3xp/ai/*.h)
file(GLOB D3GAME_ANIM src/d3xp/anim/*.cpp src/d3xp/anim/*.h)
file(GLOB D3GAME_GAMESYS src/d3xp/gamesys/*.cpp src/d3xp/gamesys/*.h)
file(GLOB D3GAME_PHYS src/d3xp/physics/*.cpp src/d3xp/physics/*.h)
file(GLOB D3GAME_SCRIPT src/d3xp/script/*.cpp src/d3xp/script/*.h)
file(GLOB D3GAME_MENUS src/d3xp/menus/*.cpp src/d3xp/menus/*.h)


file(GLOB_RECURSE AAS src/aas/*.cpp src/aas/*.h)
file(GLOB_RECURSE CM src/cm/*.cpp src/cm/*.h)
file(GLOB_RECURSE FRAMEWORK src/framework/*.cpp src/framework/*.h)
file(GLOB_RECURSE IDLIB src/idlib/*.cpp src/idlib/*.h)
file(GLOB_RECURSE RENDERER src/renderer/*.cpp src/renderer/*.h src/renderer/*.c)
file(GLOB_RECURSE SOUND src/sound/*.cpp src/sound/*.h)
file(GLOB SYS src/sys/*.cpp src/sys/*.h)
file(GLOB SYSWIN src/sys/win32/*.cpp src/sys/win32/*.h)
file(GLOB_RECURSE TOOLS src/tools/*.cpp src/tools/*.h)
file(GLOB_RECURSE UI src/ui/*.cpp src/ui/*.h)
file(GLOB SWF src/swf/*.cpp src/swf/*.h)
# file(GLOB_RECURSE ZLIB src/framework/zlib/*.c src/framework/zlib/*.h)
# file(GLOB GLAD external/glad/src/glad.c)

set(D3Source
    ${D3GAME}
    ${D3GAME_AI}
    ${D3GAME_ANIM}
    ${D3GAME_GAMESYS}
    ${D3GAME_PHYS}
    ${D3GAME_SCRIPT}
    ${D3GAME_MENUS}
    ${FRAMEWORK}
    ${IDLIB}
    ${CM}
    ${RENDERER}
    ${SOUND}
    ${SYS}
    ${SYSWIN}
    ${TOOLS}
    ${UI}
    ${GLAD}
    ${AAS}
    ${SWF}
    )


    
add_compile_options(/wd4467)
# set(CMAKE_CXX_STANDARD 17)  # Or the required C++ standard
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD_REQUIRED ON)

# add_compile_definitions(_WINDOWS)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
# add_compile_definitions(WIN32)
add_compile_definitions(_USE_32BIT_TIME_T)
add_compile_definitions(_MBCS)
add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
# add_compile_definitions(_XKEYCHECK_H)
# add_compile_definitions(_INLINEDEBUG)
add_compile_definitions(__DOOM__)
# add_compile_definitions(_DEBUG)
# add_compile_definitions(GLFW_INCLUDE_NONE)

# Needed so that VSCode can syntax check correctly.
include_directories(${CMAKE_SOURCE_DIR}/src PUBLIC external/OpenAL/include external/glfw/include external/glew/include external/zlib src/idlib)
link_directories(external/OpenAL/build/Debug external/glfw/libs external/glew/lib external/zlib/build/Release)

set(LIBS xinput dbghelp dinput8 dsound dxguid glu32 iphlpapi odbc32 odbccp32 opengl32 winmm wsock32 OpenAL32 zlibstatic glew32)

add_executable(${PROJECT_NAME} WIN32 ${D3Source})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/idLib/precompiled.h)

target_link_libraries(${PROJECT_NAME} ${LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES
LINK_FLAGS "/SUBSYSTEM:WINDOWS"
)