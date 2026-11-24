cmake_minimum_required(VERSION 3.24)
project(FoRgEEngine VERSION 0.4.0 LANGUAGES C CXX)
include(FetchContent)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(bullet3 GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git GIT_TAG 3.25 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(bullet3)
if(NOT WIN32)
  message(FATAL_ERROR "FoRgE desktop applications currently target Windows.")
endif()
if(NOT DEFINED WEBVIEW2_SDK_DIR OR NOT EXISTS "\${WEBVIEW2_SDK_DIR}/include/WebView2.h")
  message(FATAL_ERROR "WebView2 SDK not found. Set WEBVIEW2_SDK_DIR to the NuGet package build/native directory.")
endif()
set(WEBVIEW2_LOADER "\${WEBVIEW2_SDK_DIR}/x64/WebView2LoaderStatic.lib")
if(NOT EXISTS "\${WEBVIEW2_LOADER}")
  message(FATAL_ERROR "WebView2LoaderStatic.lib is missing: \${WEBVIEW2_LOADER}")
endif()
add_executable(FoRgEStudio WIN32 Studio/src/main.cpp)
target_include_directories(FoRgEStudio PRIVATE "\${WEBVIEW2_SDK_DIR}/include")
target_link_libraries(FoRgEStudio PRIVATE "\${WEBVIEW2_LOADER}" ole32 user32 version)
target_compile_definitions(FoRgEStudio PRIVATE UNICODE _UNICODE NOMINMAX)
add_executable(FoRgEPlayer Engine/src/player.cpp)
target_include_directories(FoRgEPlayer PRIVATE Engine/include "\${bullet3_SOURCE_DIR}/src")
target_link_libraries(FoRgEPlayer PRIVATE raylib BulletDynamics BulletCollision LinearMath)
add_executable(FoRgEClient WIN32 Client/src/main.cpp)
target_include_directories(FoRgEClient PRIVATE "\${WEBVIEW2_SDK_DIR}/include")
target_link_libraries(FoRgEClient PRIVATE "\${WEBVIEW2_LOADER}" ole32 user32 version)
target_compile_definitions(FoRgEClient PRIVATE UNICODE _UNICODE NOMINMAX)
set(PACKAGE "\${CMAKE_BINARY_DIR}/FoRgE-Windows-x64")
set(FONT_DIR "\${CMAKE_CURRENT_SOURCE_DIR}/WebSite/public/fonts")
set(ICON_DIR "\${CMAKE_CURRENT_SOURCE_DIR}/WebSite/public/icons")
set(PLAYER_ASSET_DIR "\${CMAKE_CURRENT_SOURCE_DIR}/Engine/assets")
set(STUDIO_UI "\${CMAKE_CURRENT_SOURCE_DIR}/Studio/UI")
add_custom_target(FoRgEPackage ALL
  COMMAND \${CMAKE_COMMAND} -E make_directory "\${PACKAGE}/Resources/Fonts"
  COMMAND \${CMAKE_COMMAND} -E make_directory "\${PACKAGE}/Resources/Icons"
  COMMAND \${CMAKE_COMMAND} -E make_directory "\${PACKAGE}/StudioUI/Resources/Fonts"
  COMMAND \${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEStudio>" "\${PACKAGE}/FoRgEStudio.exe"
  COMMAND \${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEPlayer>" "\${PACKAGE}/FoRgEPlayer.exe"
  COMMAND \${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FoRgEClient>" "\${PACKAGE}/FoRgEClient.exe"
  COMMAND \${CMAKE_COMMAND} -E copy "\${PLAYER_ASSET_DIR}/player_head.png" "\${PACKAGE}/Resources/player_head.png"
  COMMAND \${CMAKE_COMMAND} -E copy "\${PLAYER_ASSET_DIR}/player_torso.png" "\${PACKAGE}/Resources/player_torso.png"
  COMMAND \${CMAKE_COMMAND} -E copy "\${PLAYER_ASSET_DIR}/player_arm.png" "\${PACKAGE}/Resources/player_arm.png"
  COMMAND \${CMAKE_COMMAND} -E copy "\${PLAYER_ASSET_DIR}/player_leg.png" "\${PACKAGE}/Resources/player_leg.png"
  COMMAND \${CMAKE_COMMAND} -E copy "\${ICON_DIR}/forge.ico" "\${PACKAGE}/Resources/Icons/forge.ico"
  COMMAND \${CMAKE_COMMAND} -E copy "\${ICON_DIR}/forge-256.png" "\${PACKAGE}/Resources/Icons/forge-256.png"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-300.ttf" "\${PACKAGE}/Resources/Fonts/forge-300.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-400.ttf" "\${PACKAGE}/Resources/Fonts/forge-400.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-500.ttf" "\${PACKAGE}/Resources/Fonts/forge-500.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-600.ttf" "\${PACKAGE}/Resources/Fonts/forge-600.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-700.ttf" "\${PACKAGE}/Resources/Fonts/forge-700.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-800.ttf" "\${PACKAGE}/Resources/Fonts/forge-800.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-900.ttf" "\${PACKAGE}/Resources/Fonts/forge-900.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-400.ttf" "\${PACKAGE}/StudioUI/Resources/Fonts/forge-400.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-600.ttf" "\${PACKAGE}/StudioUI/Resources/Fonts/forge-600.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${FONT_DIR}/forge-800.ttf" "\${PACKAGE}/StudioUI/Resources/Fonts/forge-800.ttf"
  COMMAND \${CMAKE_COMMAND} -E copy "\${STUDIO_UI}/index.html" "\${PACKAGE}/StudioUI/index.html"
  COMMAND \${CMAKE_COMMAND} -E copy "\${STUDIO_UI}/style.css" "\${PACKAGE}/StudioUI/style.css"
  COMMAND \${CMAKE_COMMAND} -E copy "\${STUDIO_UI}/app.js" "\${PACKAGE}/StudioUI/app.js"
  COMMAND \${CMAKE_COMMAND} -E copy "\${STUDIO_UI}/three.module.js" "\${PACKAGE}/StudioUI/three.module.js"
  COMMAND \${CMAKE_COMMAND} -E copy "\${STUDIO_UI}/OrbitControls.js" "\${PACKAGE}/StudioUI/OrbitControls.js"
  COMMAND \${CMAKE_COMMAND} -E copy "\${CMAKE_CURRENT_SOURCE_DIR}/Studio/README.md" "\${PACKAGE}/README.md"
  DEPENDS FoRgEStudio FoRgEPlayer FoRgEClient
  VERBATIM
)