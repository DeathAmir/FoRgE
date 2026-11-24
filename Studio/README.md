# FoRgE Creator Studio

Native Windows x64 creator workspace built with C++20, Win32, Direct3D 11 and Dear ImGui.

## Distribution layout

FoRgE Studio is distributed as a folder, not a standalone EXE:

- FoRgEStudio.exe
- Resources/Fonts/forge-300.ttf through forge-900.ttf
- Resources/Icons/forge.ico
- README.md

Keep Resources beside the executable. All seven FoRgE font weights are loaded at startup. The ICO is embedded as the native Windows application icon and is also included in the package.

## Build

Requires Visual Studio 2022 with the Desktop development with C++ workload and CMake.

    cmake -S Studio -B Studio/build -A x64
    cmake --build Studio/build --config Release --parallel

The distributable folder is Studio/build/FoRgEStudio-Package. The build workflow generates the FoRgE font and icon assets from tools/build_brand_assets.py before compiling.

## Included

- Custom borderless FoRgE title bar with minimize, maximize/restore and close controls
- Resizable native window and custom application icon
- Welcome screen with Open Studio and New Project entry points
- All seven FoRgE font weights (300, 400, 500, 600, 700, 800, 900)
- Typography preview and font-weight selector
- Explorer, viewport, inspector and output panels
- Initial play/stop state and editor selection

The FoRgE type family is based on Barlow Condensed, distributed under the SIL Open Font License, with FoRgE brand modifications and glyphs.
