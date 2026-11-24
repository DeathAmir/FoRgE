
cmake_minimum_required(VERSION 3.24)
project(FoRgEEngine VERSION 0.3.0 LANGUAGES C CXX)
include(FetchContent)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(raygui URL https://github.com/raysan5/raygui/archive/refs/tags/4.0.zip DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_GetProperties(raygui)
if(NOT raygui_POPULATED)
  FetchContent_Populate(raygui)
endif()
add_executable(FoRgEStudio Studio/src/main.cpp)
target_include_directories(FoRgEStudio PRIVATE Engine/include "${raygui_SOURCE_DIR}/src")
target_link_libraries(FoRgEStudio PRIVATE raylib)
add_executable(FoRgEPlayer Engine/src/player.cpp)
target_include_directories(FoRgEPlayer PRIVATE Engine/include)
target_link_libraries(FoRgEPlayer PRIVATE raylib)
if(NOT WIN32)
  message(FATAL_ERROR "FoRgE desktop client currently targets Windows.")
endif()
if(NOT DEFINED WEBVIEW2_SDK_DIR OR NOT EXISTS "${WEBVIEW2_SDK_DIR}/include/WebView2.h")
  message(FATAL_ERROR "WebView2 SDK not found. Set WEBVIEW2_SDK_DIR to the NuGet package build/native directory.")
endif()
set(WEBVIEW2_LOADER "${WEBVIEW2_SDK_DIR}/x64/WebView2LoaderStatic.lib")
if(NOT EXISTS "${WEBVIEW2_LOADER}")
  message(FATAL_ERROR "WebView2LoaderStatic.lib is missing: ${WEBVIEW2_LOADER}")
endif()
add_executable(FoRgEClient WIN32 Client/src/main.cpp)
target_include_directories(FoRgEClient PRIVATE "${WEBVIEW2_SDK_DIR}/include")
target_link_libraries(FoRgEClient PRIVATE "${WEBVIEW2_LOADER}" ole32 user32)
target_compile_definitions(FoRgEClient PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
set(PACKAGE "${CMAKE_BINARY_DIR}/FoRgE-Windows-x64")
set(FONT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/WebSite/public/fonts")
set(ICON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/WebSite/public/icons")
set(PLAYER_TEXTURE "${CMAKE_CURRENT_SOURCE_DIR}/Engine/assets/player_texture.png")
add_custom_target(FoRgEPackage ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE}/Resources/Fonts"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE}/Resources/Icons"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEStudio>" "${PACKAGE}/FoRgEStudio.exe"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEPlayer>" "${PACKAGE}/FoRgEPlayer.exe"
  COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEClient>" "${PACKAGE}/FoRgEClient.exe"
  COMMAND ${CMAKE_COMMAND} -E copy "${PLAYER_TEXTURE}" "${PACKAGE}/Resources/player_texture.png"
  COMMAND ${CMAKE_COMMAND} -E copy "${ICON_DIR}/forge.ico" "${PACKAGE}/Resources/Icons/forge.ico"
  COMMAND ${CMAKE_COMMAND} -E copy "${ICON_DIR}/forge-256.png" "${PACKAGE}/Resources/Icons/forge-256.png"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-300.ttf" "${PACKAGE}/Resources/Fonts/forge-300.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-400.ttf" "${PACKAGE}/Resources/Fonts/forge-400.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-500.ttf" "${PACKAGE}/Resources/Fonts/forge-500.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-600.ttf" "${PACKAGE}/Resources/Fonts/forge-600.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-700.ttf" "${PACKAGE}/Resources/Fonts/forge-700.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-800.ttf" "${PACKAGE}/Resources/Fonts/forge-800.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${FONT_DIR}/forge-900.ttf" "${PACKAGE}/Resources/Fonts/forge-900.ttf"
  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Studio/README.md" "${PACKAGE}/README.md"
  DEPENDS FoRgEStudio FoRgEPlayer FoRgEClient
  VERBATIM
)
