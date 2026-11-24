# FoRgE Creator Studio and Runtime

The native C++ foundation of the FoRgE creation platform.

- **FoRgEStudio.exe** — raylib + raygui editor, live 3D viewport, block placement/removal, copper/stone/grass materials, textured six-part block character, inspector, all seven FoRgE font weights and scene save.
- **FoRgEPlayer.exe** — native C++ runtime prototype with third-person camera, WASD movement, jumping and loading the saved local scene.
- **FoRgEClient.exe** — Windows WebView2 desktop shell. Play and Creator Studio launch the native applications.

The unified Windows workflow generates brand fonts, ICO/PNG icons and the pixel-art player texture, installs the WebView2 SDK, configures CMake and builds all three applications.

For local builds, install CMake, Visual Studio 2022 C++ tools, Python 3.12 and the WebView2 SDK NuGet package, then run CMake with `-DWEBVIEW2_SDK_DIR=<package>/build/native`.

The editor saves `scene.forge` beside the applications. This is an early engine foundation, not a finished online platform: networking, accounts, publishing, physics and collaborative editing are not implemented yet.
