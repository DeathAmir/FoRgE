# FoRgE Creator Studio

Initial native Windows x64 creator application built with C++20, Direct3D 11, Win32 and a fully custom FoRgE ImGui theme and editor layout.

## Build

Requires Visual Studio 2022 with the Desktop development with C++ workload and CMake.

```powershell
cmake -S Studio -B Studio/build -A x64
cmake --build Studio/build --config Release
```

The Windows GitHub Actions workflow builds and publishes `FoRgEStudio.exe` as an artifact.

## Current foundation

- Native Win32 application and Direct3D 11 renderer
- Custom FoRgE dark/copper interface styling
- Explorer, viewport, inspector and output panels
- Initial play/stop state and editor selection
- Dear ImGui docking-branch source built as a dependency; no default ImGui demo UI is exposed
