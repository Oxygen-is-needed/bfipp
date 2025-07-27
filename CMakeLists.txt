cmake_minimum_required(VERSION 3.14)

project(bfipp LANGUAGES CXX VERSION 1.1.0 )
set(VRESION_PRE "")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)


set(CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pkg)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pkg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pkg)


# Disable Options
option(DISABLE_GRAPHICS "Disable Simple Graphics" OFF)
## Frontends Disable Options
option(DISABLE_SIMPLE_TEXT "Disable Simple Text Frontend" OFF)


# Windows Build Static
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(BUILD_FOR_WIN TRUE)
endif()

# Static Build
option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})
if (STATIC_BUILD)
        set(CMAKE_EXE_LINKER_FLAGS "-static")
        set(CMAKE_FIND_LIBRARY_SUFFIXrS ".a" CONFIG)
        set(BUILD_SHARED_LIBS OFF)
endif()

# raygui {{{
include(FetchContent)

function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
    target_compile_options(${libName} PRIVATE "-w")
endfunction()

set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG        4.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_GetProperties(raygui)
if(NOT raygui_POPULATED)
    FetchContent_MakeAvailable(raygui)
    add_subdirectory(${raygui_SOURCE_DIR}/projects/CMake raygui)
    target_compile_options(raygui INTERFACE "-w")
endif()
# }}}

add_executable(bfi++ ${CMAKE_SOURCE_DIR}/main.cpp)
install(TARGETS bfi++ DESTINATION bin)

target_link_libraries(bfi++ PRIVATE raygui raylib)


# Windows Stuff
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_libraries(bfi++ PRIVATE "-lstdc++exp")
endif()

# Disable
if (DISABLE_GRAPHICS)
	target_compile_definitions(bfi++ PRIVATE
		"-DDISABLE_GRAPHICS")
endif()
## Disable Frontends Macros
if (DISABLE_SIMPLE_TEXT)
	target_compile_definitions(bfi++ PRIVATE
		"-DDISABLE_FRONTEND__SIMPLE_TEXT")
endif()

target_precompile_headers(bfi++ PRIVATE <filesystem> <fstream> <getopt.h>
	<iostream> <limits> <print> <stack> <string> <vector> <cctype>)

target_compile_definitions(bfi++ PRIVATE
	VERSION="v${bfipp_VERSION_MAJOR}.${bfipp_VERSION_MINOR}.${bfipp_VERSION_PATCH}${VERSION_PRE}")
