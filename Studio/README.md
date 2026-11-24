# FoRgE Creator Studio

FoRgE Studio is a native Win32 application whose complete editor interface is hosted by Microsoft Edge WebView2. The editor uses the FoRgE typeface and a local Three.js WebGL viewport; it does not use raylib GUI or raygui.

## Editor controls

- Select a material in Block Library, then left-click the scene to place it.
- Left-click a block to select it.
- Right-click a block to remove it.
- Middle-drag to orbit and use the wheel to zoom.
- `Ctrl+S` saves `scene.forge` beside the application.
- Play Test saves the current scene and launches the native C++ player.

## Runtime

The player uses raylib for rendering and Bullet Physics for the character controller, gravity, ground contact, jumping, and collisions with world blocks. The default character is assembled from separately textured head, torso, arms, and legs with a procedural walk cycle.

## Build

Configure from the repository root with CMake and the WebView2 SDK directory:

`cmake -S . -B build -A x64 -DWEBVIEW2_SDK_DIR=<WebView2 native SDK path>`

The Windows workflow installs the WebView2 SDK and local Three.js files, builds all native applications, and packages the FoRgE font files and character textures. WebView2 Runtime must be installed on the target Windows machine.