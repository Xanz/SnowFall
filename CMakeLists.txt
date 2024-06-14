cmake_minimum_required(VERSION 3.13.0)
project(SnowFall VERSION 1.4.3)

file(GLOB D3GAME src/game/*.cpp src/game/*.h src/game/game.def)
file(GLOB D3GAME_AI src/game/ai/*.cpp src/game/ai/*.h)
file(GLOB D3GAME_ANIM src/game/anim/*.cpp src/game/anim/*.h)
file(GLOB D3GAME_GAMESYS src/game/gamesys/*.cpp src/game/gamesys/*.h)
file(GLOB D3GAME_PHYS src/game/physics/*.cpp src/game/physics/*.h)
file(GLOB D3GAME_SCRIPT src/game/script/*.cpp src/game/script/*.h)

file(GLOB_RECURSE CM src/cm/*.cpp src/cm/*.h)
file(GLOB_RECURSE FRAMEWORK src/framework/*.cpp src/framework/*.h)
file(GLOB_RECURSE IDLIB src/idlib/*.cpp src/idlib/*.h)
file(GLOB_RECURSE RENDERER src/renderer/*.cpp src/renderer/*.h src/renderer/*.c)
file(GLOB_RECURSE SOUND src/sound/*.cpp src/sound/*.h)
file(GLOB SYS src/sys/*.cpp src/sys/*.h)
file(GLOB SYSWIN src/sys/win32/*.cpp src/sys/win32/*.h)
file(GLOB_RECURSE TOOLS src/tools/*.cpp src/tools/*.h)
file(GLOB_RECURSE UI src/ui/*.cpp src/ui/*.h)

# New Snowfall Renderer backend.
file(GLOB_RECURSE SFRENDERER src/sfRenderer/*.cpp src/sfRenderer/*.h)

# New Snowfall UI system. (Powered by RMLUI.)
file(GLOB_RECURSE SFUI src/sfUI/*.cpp src/sfUI/*.h)

set(D3Source
    ${D3GAME}
    ${D3GAME_AI}
    ${D3GAME_ANIM}
    ${D3GAME_GAMESYS}
    ${D3GAME_PHYS}
    ${D3GAME_SCRIPT}
    ${FRAMEWORK}
    ${IDLIB}
    ${CM}
    ${RENDERER}
    ${SFRENDERER}
    ${SFUI}
    ${SOUND}
    ${SYS}
    ${SYSWIN}
    ${TOOLS}
    ${UI}
    ${GLAD}
    )

add_compile_definitions(_WINDOWS)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(WIN32)
add_compile_definitions(_XKEYCHECK_H)
add_compile_definitions(USE_OPENAL)



# Built as a sub directory should be moved to build as part of dependency scripts.
add_subdirectory(external/RmlUi)

add_definitions(-DRMLUI_STATIC_LIB)

include_directories(${CMAKE_SOURCE_DIR}/src PUBLIC external/openal/include external/glfw/include external/glew/include)
link_directories(external/openal/build/Release external/glfw/build/src/Release external/glew/lib/Release/x64)

set(LIBS dbghelp dinput8 dxguid glu32 iphlpapi opengl32 winmm ws2_32 OpenAL32 odbc32 odbccp32 glfw3 glew32 RmlUi::RmlUi)

add_executable(${PROJECT_NAME} WIN32 ${D3Source})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/idLib/precompiled.h)

target_link_libraries(${PROJECT_NAME} ${LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES
LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
)