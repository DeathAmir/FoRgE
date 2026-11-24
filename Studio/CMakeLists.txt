cmake_minimum_required(VERSION 3.24)
project(FoRgEStudio VERSION 0.2.0 LANGUAGES CXX RC)
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG docking GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(imgui)
add_library(imgui_core ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)
target_include_directories(imgui_core PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_definitions(imgui_core PUBLIC UNICODE _UNICODE)
if(MSVC)
 set_property(TARGET imgui_core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
if(WIN32)
 set(FORGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../WebSite/public/icons/forge.ico")
 set(FORGE_RC "${CMAKE_CURRENT_BINARY_DIR}/generated/forge_icon.rc")
 file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")
 configure_file(resources/forge_icon.rc.in "${FORGE_RC}" @ONLY)
 add_executable(FoRgEStudio WIN32 src/main.cpp resources/resource.h "${FORGE_RC}")
 target_link_libraries(FoRgEStudio PRIVATE imgui_core d3d11 dxgi dwmapi)
 target_compile_definitions(FoRgEStudio PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
 if(MSVC)
  target_compile_options(FoRgEStudio PRIVATE /W4 /permissive- /EHsc)
  set_property(TARGET FoRgEStudio PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
 endif()
 set(PACKAGE "${CMAKE_BINARY_DIR}/FoRgEStudio-Package")
 set(FONTS "${CMAKE_CURRENT_SOURCE_DIR}/../WebSite/public/fonts")
 set(ICON "${CMAKE_CURRENT_SOURCE_DIR}/../WebSite/public/icons/forge.ico")
 add_custom_command(TARGET FoRgEStudio POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE}/Resources/Fonts"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE}/Resources/Icons"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEStudio>" "${PACKAGE}/FoRgEStudio.exe"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-300.ttf" "${PACKAGE}/Resources/Fonts/forge-300.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-400.ttf" "${PACKAGE}/Resources/Fonts/forge-400.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-500.ttf" "${PACKAGE}/Resources/Fonts/forge-500.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-600.ttf" "${PACKAGE}/Resources/Fonts/forge-600.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-700.ttf" "${PACKAGE}/Resources/Fonts/forge-700.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-800.ttf" "${PACKAGE}/Resources/Fonts/forge-800.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONTS}/forge-900.ttf" "${PACKAGE}/Resources/Fonts/forge-900.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${ICON}" "${PACKAGE}/Resources/Icons/forge.ico"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/README.md" "${PACKAGE}/README.md"
  VERBATIM)
else()
 message(FATAL_ERROR "FoRgE Studio currently targets Windows only.")
endif()
