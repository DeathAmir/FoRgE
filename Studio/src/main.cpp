#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dwmapi.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwmapi.lib")

static ID3D11Device* g_device = nullptr;
static ID3D11DeviceContext* g_context = nullptr;
static IDXGISwapChain* g_swapChain = nullptr;
static ID3D11RenderTargetView* g_target = nullptr;
static bool g_running = true;
static int g_selected = 0;
static bool g_playing = false;
static std::vector<std::string> g_console = {"FoRgE Studio 0.1.0 initialized.", "Creator workspace ready.", "No project loaded."};

static void CreateTarget() {
    ID3D11Texture2D* buffer = nullptr;
    g_swapChain->GetBuffer(0, IID_PPV_ARGS(&buffer));
    if (buffer) {
        g_device->CreateRenderTargetView(buffer, nullptr, &g_target);
        buffer->Release();
    }
}

static void CleanupTarget() {
    if (g_target) {
        g_target->Release();
        g_target = nullptr;
    }
}

static bool CreateDevice(HWND window) {
    DXGI_SWAP_CHAIN_DESC desc{};
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = window;
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    D3D_FEATURE_LEVEL level{};
    const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};
    HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, 2,
        D3D11_SDK_VERSION, &desc, &g_swapChain, &g_device, &level, &g_context);
    if (FAILED(result)) return false;
    CreateTarget();
    return true;
}

static void ApplyTheme() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 8.0f;
    s.ChildRounding = 7.0f;
    s.FrameRounding = 5.0f;
    s.PopupRounding = 6.0f;
    s.ScrollbarRounding = 8.0f;
    s.GrabRounding = 5.0f;
    s.TabRounding = 5.0f;
    s.WindowPadding = ImVec2(12, 10);
    s.FramePadding = ImVec2(9, 6);
    s.ItemSpacing = ImVec2(8, 7);
    s.WindowBorderSize = 0.0f;
    s.ChildBorderSize = 1.0f;
    s.FrameBorderSize = 0.0f;
    s.Colors[ImGuiCol_WindowBg] = ImVec4(0.047f, 0.052f, 0.064f, 1.0f);
    s.Colors[ImGuiCol_ChildBg] = ImVec4(0.064f, 0.071f, 0.086f, 1.0f);
    s.Colors[ImGuiCol_PopupBg] = ImVec4(0.075f, 0.081f, 0.097f, 0.98f);
    s.Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.16f, 0.19f, 1.0f);
    s.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.11f, 0.13f, 1.0f);
    s.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.17f, 0.20f, 1.0f);
    s.Colors[ImGuiCol_Button] = ImVec4(0.62f, 0.20f, 0.13f, 1.0f);
    s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.78f, 0.29f, 0.18f, 1.0f);
    s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.48f, 0.15f, 0.11f, 1.0f);
    s.Colors[ImGuiCol_Header] = ImVec4(0.35f, 0.13f, 0.11f, 1.0f);
    s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.52f, 0.18f, 0.13f, 1.0f);
    s.Colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.43f, 0.25f, 1.0f);
    s.Colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.32f, 0.20f, 1.0f);
    s.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.48f, 0.27f, 1.0f);
    s.Colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.11f, 0.13f, 1.0f);
    s.Colors[ImGuiCol_TabSelected] = ImVec4(0.35f, 0.14f, 0.11f, 1.0f);
    s.Colors[ImGuiCol_TitleBg] = ImVec4(0.047f, 0.052f, 0.064f, 1.0f);
    s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.047f, 0.052f, 0.064f, 1.0f);
}

static void Panel(const char* title, ImVec2 pos, ImVec2 size, ImGuiWindowFlags flags = 0) {
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::Begin(title, nullptr, flags | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
}

static void DrawEditor() {
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    const float top = 48.0f;
    const float bottom = 190.0f;
    const float left = 238.0f;
    const float right = 270.0f;
    const float centerW = std::max(120.0f, display.x - left - right);
    const float centerH = std::max(100.0f, display.y - top - bottom);

    Panel("##TopBar", ImVec2(0, 0), ImVec2(display.x, top), ImGuiWindowFlags_NoTitleBar);
    ImGui::TextColored(ImVec4(0.96f, 0.38f, 0.23f, 1), "F");
    ImGui::SameLine();
    ImGui::Text("FoRgE");
    ImGui::SameLine(105);
    ImGui::TextDisabled("File   Edit   View   Build   Window   Help");
    ImGui::SameLine(display.x - 230);
    if (ImGui::Button(g_playing ? "  STOP  " : "  PLAY  ")) g_playing = !g_playing;
    ImGui::SameLine();
    ImGui::TextDisabled("Creator Studio  |  0.1.0");
    ImGui::End();

    Panel("Explorer", ImVec2(0, top), ImVec2(left, centerH));
    ImGui::TextDisabled("WORKSPACE");
    ImGui::Separator();
    if (ImGui::Selectable("  Scene", g_selected == 0)) g_selected = 0;
    if (ImGui::Selectable("  Workspace", g_selected == 1)) g_selected = 1;
    if (ImGui::Selectable("  Lighting", g_selected == 2)) g_selected = 2;
    if (ImGui::Selectable("  Camera", g_selected == 3)) g_selected = 3;
    ImGui::Spacing();
    ImGui::TextDisabled("ASSETS");
    ImGui::Separator();
    ImGui::BulletText("Materials");
    ImGui::BulletText("Meshes");
    ImGui::BulletText("Textures");
    ImGui::BulletText("Scripts");
    ImGui::End();

    Panel("Inspector", ImVec2(display.x - right, top), ImVec2(right, centerH));
    ImGui::TextDisabled("PROPERTIES");
    ImGui::Separator();
    ImGui::Text("Selection");
    ImGui::TextDisabled(g_selected == 0 ? "Scene" : g_selected == 1 ? "Workspace" : g_selected == 2 ? "Lighting" : "Camera");
    ImGui::Spacing();
    ImGui::Text("Transform");
    static float position[3] = {0.0f, 0.0f, 0.0f};
    static float rotation[3] = {0.0f, 0.0f, 0.0f};
    static float scale[3] = {1.0f, 1.0f, 1.0f};
    ImGui::DragFloat3("Position", position, 0.1f);
    ImGui::DragFloat3("Rotation", rotation, 1.0f);
    ImGui::DragFloat3("Scale", scale, 0.05f);
    ImGui::End();

    Panel("Viewport", ImVec2(left, top), ImVec2(centerW, centerH));
    ImGui::TextDisabled("PERSPECTIVE   |   EDITOR CAMERA");
    ImGui::SameLine(centerW - 90);
    ImGui::TextDisabled(g_playing ? "RUNNING" : "EDIT MODE");
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(left + 12, top + 58), ImVec2(left + centerW - 12, top + centerH - 12), IM_COL32(17, 21, 28, 255), 7.0f);
    ImGui::SetCursorPosY(centerH * 0.48f);
    ImGui::SetCursorPosX(centerW * 0.5f - 112);
    ImGui::TextColored(ImVec4(0.87f, 0.35f, 0.22f, 1), "FoRgE");
    ImGui::SameLine();
    ImGui::TextDisabled("Viewport");
    ImGui::SetCursorPosX(centerW * 0.5f - 128);
    ImGui::TextDisabled("Your world starts here.");
    ImGui::End();

    Panel("Output", ImVec2(0, display.y - bottom), ImVec2(display.x, bottom));
    ImGui::TextDisabled("OUTPUT   |   CONSOLE");
    ImGui::Separator();
    for (const auto& line : g_console) ImGui::TextUnformatted(line.c_str());
    ImGui::End();
}

static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    switch (msg) {
    case WM_SIZE:
        if (g_device && wParam != SIZE_MINIMIZED) {
            CleanupTarget();
            g_swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int show) {
    WNDCLASSEX wc{sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0, instance, nullptr, nullptr, nullptr, nullptr, _T("FoRgEStudioWindow"), nullptr};
    RegisterClassEx(&wc);
    HWND window = CreateWindow(wc.lpszClassName, _T("FoRgE Creator Studio"), WS_OVERLAPPEDWINDOW, 100, 80, 1500, 940, nullptr, nullptr, wc.hInstance, nullptr);
    if (!CreateDevice(window)) {
        CleanupTarget();
        if (g_swapChain) g_swapChain->Release();
        if (g_context) g_context->Release();
        if (g_device) g_device->Release();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    ShowWindow(window, show);
    UpdateWindow(window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ApplyTheme();
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(g_device, g_context);

    MSG message{};
    while (message.message != WM_QUIT) {
        if (PeekMessage(&message, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
            continue;
        }
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        DrawEditor();
        ImGui::Render();
        const float clear[4] = {0.035f, 0.039f, 0.048f, 1.0f};
        g_context->OMSetRenderTargets(1, &g_target, nullptr);
        g_context->ClearRenderTargetView(g_target, clear);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_swapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupTarget();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    DestroyWindow(window);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
