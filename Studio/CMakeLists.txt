cmake_minimum_required(VERSION 3.24)
project(FoRgEStudio VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)

add_library(imgui_core
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)
target_include_directories(imgui_core PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_definitions(imgui_core PUBLIC UNICODE _UNICODE)

if(WIN32)
    add_executable(FoRgEStudio WIN32 src/main.cpp)
    target_link_libraries(FoRgEStudio PRIVATE imgui_core d3d11 dxgi dwmapi)
    target_compile_definitions(FoRgEStudio PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
    if(MSVC)
        target_compile_options(FoRgEStudio PRIVATE /W4 /permissive- /EHsc)
        set_property(TARGET FoRgEStudio PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
else()
    message(FATAL_ERROR "FoRgE Studio currently targets Windows only.")
endif()
