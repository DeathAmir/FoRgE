#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dwmapi.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <array>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../resources/resource.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwmapi.lib")

static ID3D11Device* g_device=nullptr;
static ID3D11DeviceContext* g_context=nullptr;
static IDXGISwapChain* g_swapChain=nullptr;
static ID3D11RenderTargetView* g_target=nullptr;
static HWND g_window=nullptr;
static bool g_playing=false, g_welcome=true, g_maximized=false;
static int g_selected=0, g_fontChoice=3;
static std::array<ImFont*,7> g_fonts{};
static const char* g_fontNames[]={"Light 300","Regular 400","Medium 500","SemiBold 600","Bold 700","ExtraBold 800","Black 900"};
static const char* g_fontFiles[]={"forge-300.ttf","forge-400.ttf","forge-500.ttf","forge-600.ttf","forge-700.ttf","forge-800.ttf","forge-900.ttf"};
static std::vector<std::string> g_console={"FoRgE Creator Studio 0.2.0 initialized.","Brand typography loaded.","Welcome to your creator workspace."};

static std::filesystem::path ExeDir(){wchar_t p[MAX_PATH]{};GetModuleFileNameW(nullptr,p,MAX_PATH);return std::filesystem::path(p).parent_path();}
static void CreateTarget(){ID3D11Texture2D* b=nullptr;if(SUCCEEDED(g_swapChain->GetBuffer(0,IID_PPV_ARGS(&b)))&&b){g_device->CreateRenderTargetView(b,nullptr,&g_target);b->Release();}}
static void CleanupTarget(){if(g_target){g_target->Release();g_target=nullptr;}}
static bool CreateDevice(HWND w){
 DXGI_SWAP_CHAIN_DESC d{};d.BufferCount=2;d.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;d.BufferDesc.RefreshRate={60,1};d.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;d.OutputWindow=w;d.SampleDesc.Count=1;d.Windowed=TRUE;d.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
 D3D_FEATURE_LEVEL l{};const D3D_FEATURE_LEVEL levels[]={D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_0};
 if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,0,levels,2,D3D11_SDK_VERSION,&d,&g_swapChain,&g_device,&l,&g_context)))return false;CreateTarget();return true;
}
static void Theme(){
 ImGuiStyle&s=ImGui::GetStyle();s.WindowRounding=0;s.ChildRounding=8;s.FrameRounding=5;s.PopupRounding=6;s.ScrollbarRounding=8;s.GrabRounding=5;s.TabRounding=5;s.WindowPadding={14,12};s.FramePadding={10,7};s.ItemSpacing={9,8};s.WindowBorderSize=0;s.ChildBorderSize=1;s.FrameBorderSize=0;
 s.Colors[ImGuiCol_WindowBg]={0.047f,0.052f,0.064f,1};s.Colors[ImGuiCol_ChildBg]={0.064f,0.071f,0.086f,1};s.Colors[ImGuiCol_PopupBg]={0.075f,0.081f,0.097f,0.98f};s.Colors[ImGuiCol_Border]={0.15f,0.16f,0.19f,1};s.Colors[ImGuiCol_FrameBg]={0.10f,0.11f,0.13f,1};s.Colors[ImGuiCol_FrameBgHovered]={0.16f,0.17f,0.20f,1};s.Colors[ImGuiCol_Button]={0.62f,0.20f,0.13f,1};s.Colors[ImGuiCol_ButtonHovered]={0.78f,0.29f,0.18f,1};s.Colors[ImGuiCol_ButtonActive]={0.48f,0.15f,0.11f,1};s.Colors[ImGuiCol_Header]={0.35f,0.13f,0.11f,1};s.Colors[ImGuiCol_HeaderHovered]={0.52f,0.18f,0.13f,1};s.Colors[ImGuiCol_CheckMark]={0.95f,0.43f,0.25f,1};s.Colors[ImGuiCol_SliderGrab]={0.88f,0.32f,0.20f,1};s.Colors[ImGuiCol_SliderGrabActive]={1,0.48f,0.27f,1};s.Colors[ImGuiCol_Tab]={0.10f,0.11f,0.13f,1};s.Colors[ImGuiCol_TabSelected]={0.35f,0.14f,0.11f,1};
}
static bool LoadFonts(){
 ImGuiIO&io=ImGui::GetIO();auto dir=ExeDir()/L"Resources"/L"Fonts";
 for(size_t i=0;i<g_fonts.size();++i){auto p=dir/g_fontFiles[i];g_fonts[i]=io.Fonts->AddFontFromFileTTF(p.string().c_str(),i==3?17.0f:16.0f);if(!g_fonts[i])return false;}
 return io.Fonts->Build();
}
static void TitleBar(){
 ImVec2 d=ImGui::GetIO().DisplaySize;ImGui::SetNextWindowPos({0,0});ImGui::SetNextWindowSize({d.x,46});ImGui::Begin("##TitleBar",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
 ImGui::PushFont(g_fonts[6]);ImGui::TextColored({0.96f,0.43f,0.27f,1},"F");ImGui::PopFont();ImGui::SameLine(38);ImGui::SetCursorPosY(13);ImGui::Text("FoRgE");ImGui::SameLine(110);ImGui::SetCursorPosY(15);ImGui::TextDisabled("CREATOR STUDIO / 0.2.0");
 ImGui::SameLine(d.x-132);ImGui::SetCursorPosY(7);if(ImGui::Button("_",{36,31}))ShowWindow(g_window,SW_MINIMIZE);ImGui::SameLine();if(ImGui::Button(g_maximized?"❐":"□",{36,31})){g_maximized=!g_maximized;ShowWindow(g_window,g_maximized?SW_MAXIMIZE:SW_RESTORE);}ImGui::SameLine();ImGui::PushStyleColor(ImGuiCol_Button,{0.55f,0.12f,0.10f,1});ImGui::PushStyleColor(ImGuiCol_ButtonHovered,{0.82f,0.16f,0.13f,1});if(ImGui::Button("×",{36,31}))PostMessageW(g_window,WM_CLOSE,0,0);ImGui::PopStyleColor(2);
 if(ImGui::IsWindowHovered()&&ImGui::IsMouseDragging(ImGuiMouseButton_Left)&&!ImGui::IsAnyItemHovered()){ImVec2 delta=ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);if(delta.x||delta.y){RECT r{};GetWindowRect(g_window,&r);SetWindowPos(g_window,nullptr,r.left+(int)delta.x,r.top+(int)delta.y,0,0,SWP_NOSIZE|SWP_NOZORDER);ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);}}
 ImGui::End();
}
static void Welcome(){
 ImVec2 d=ImGui::GetIO().DisplaySize;float w=std::min(850.0f,d.x-80);ImGui::SetNextWindowPos({0,46});ImGui::SetNextWindowSize({d.x,d.y-46});ImGui::Begin("##Welcome",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
 ImGui::SetCursorPos({(d.x-w)*.5f,64});ImGui::BeginChild("##Hero",{w,250},true);ImGui::SetCursorPosY(25);ImGui::PushFont(g_fonts[6]);ImGui::TextColored({0.95f,0.39f,0.24f,1},"FoRgE");ImGui::PopFont();ImGui::SameLine();ImGui::SetCursorPosY(37);ImGui::TextDisabled("CREATOR STUDIO");ImGui::Spacing();ImGui::PushFont(g_fonts[5]);ImGui::Text("Build something worth playing.");ImGui::PopFont();ImGui::TextWrapped("Your creative workspace is ready. Start a new world or open the editor and continue building.");ImGui::Spacing();
 if(ImGui::Button("  OPEN STUDIO  ",{170,42})){g_welcome=false;g_console.push_back("Creator workspace opened.");}ImGui::SameLine();if(ImGui::Button("  NEW PROJECT  ",{150,42})){g_welcome=false;g_console.push_back("New empty workspace created.");}ImGui::EndChild();
 ImGui::SetCursorPos({(d.x-w)*.5f,335});ImGui::BeginChild("##Typography",{w,260},true);ImGui::TextDisabled("YOUR BRAND TYPE SYSTEM");ImGui::Separator();for(int i=0;i<7;i++){ImGui::PushFont(g_fonts[i]);ImGui::Text("%s",g_fontNames[i]);ImGui::PopFont();ImGui::SameLine(190);ImGui::TextDisabled("FoRgE Studio 0123456789");}ImGui::EndChild();ImGui::SetCursorPos({(d.x-w)*.5f,d.y-48});ImGui::TextDisabled("FoRgE / DESIGNED TO CREATE");ImGui::End();
}
static void Panel(const char*t,ImVec2 p,ImVec2 s){ImGui::SetNextWindowPos(p);ImGui::SetNextWindowSize(s);ImGui::Begin(t,nullptr,ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize);}
static void Editor(){
 ImVec2 d=ImGui::GetIO().DisplaySize;float top=46,bottom=190,left=238,right=270,cw=std::max(120.0f,d.x-left-right),ch=std::max(100.0f,d.y-top-bottom);
 ImGui::SetNextWindowPos({0,top});ImGui::SetNextWindowSize({left,ch});ImGui::Begin("Explorer");ImGui::TextDisabled("WORKSPACE");ImGui::Separator();if(ImGui::Selectable("Scene",g_selected==0))g_selected=0;if(ImGui::Selectable("Workspace",g_selected==1))g_selected=1;if(ImGui::Selectable("Lighting",g_selected==2))g_selected=2;if(ImGui::Selectable("Camera",g_selected==3))g_selected=3;ImGui::Spacing();ImGui::TextDisabled("ASSETS");ImGui::Separator();ImGui::BulletText("Materials");ImGui::BulletText("Meshes");ImGui::BulletText("Textures");ImGui::BulletText("Scripts");ImGui::End();
 Panel("Inspector",{d.x-right,top},{right,ch});ImGui::TextDisabled("PROPERTIES");ImGui::Separator();ImGui::Text("Selection");ImGui::TextDisabled(g_selected==0?"Scene":g_selected==1?"Workspace":g_selected==2?"Lighting":"Camera");static float p[3]{},r[3]{},s[3]{1,1,1};ImGui::Text("Transform");ImGui::DragFloat3("Position",p,.1f);ImGui::DragFloat3("Rotation",r,1);ImGui::DragFloat3("Scale",s,.05f);ImGui::Spacing();ImGui::TextDisabled("INTERFACE FONT");if(ImGui::BeginCombo("Weight",g_fontNames[g_fontChoice])){for(int i=0;i<7;i++)if(ImGui::Selectable(g_fontNames[i],g_fontChoice==i))g_fontChoice=i;ImGui::EndCombo();}ImGui::End();
 Panel("Viewport",{left,top},{cw,ch});ImGui::TextDisabled("PERSPECTIVE / EDITOR CAMERA");ImGui::SameLine(cw-90);ImGui::TextDisabled(g_playing?"RUNNING":"EDIT MODE");ImGui::GetWindowDrawList()->AddRectFilled({left+12,top+58},{left+cw-12,top+ch-12},IM_COL32(17,21,28,255),7);ImGui::SetCursorPosY(ch*.48f);ImGui::SetCursorPosX(cw*.5f-110);ImGui::PushFont(g_fonts[6]);ImGui::TextColored({0.87f,0.35f,0.22f,1},"FoRgE");ImGui::PopFont();ImGui::SameLine();ImGui::TextDisabled("Viewport");ImGui::SetCursorPosX(cw*.5f-128);ImGui::TextDisabled("Your world starts here.");ImGui::End();
 Panel("Output",{0,d.y-bottom},{d.x,bottom});ImGui::TextDisabled("OUTPUT / CONSOLE");ImGui::Separator();for(auto&line:g_console)ImGui::TextUnformatted(line.c_str());ImGui::End();
 ImGui::SetNextWindowPos({0,46});ImGui::SetNextWindowSize({d.x,0});ImGui::Begin("##Toolbar",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_AlwaysAutoResize);if(ImGui::Button("FILE"))ImGui::OpenPopup("FileMenu");if(ImGui::BeginPopup("FileMenu")){if(ImGui::MenuItem("Welcome"))g_welcome=true;if(ImGui::MenuItem("Exit"))PostMessageW(g_window,WM_CLOSE,0,0);ImGui::EndPopup();}ImGui::SameLine();ImGui::TextDisabled("Edit    View    Build");ImGui::SameLine(d.x-190);if(ImGui::Button(g_playing?"STOP":"PLAY",{80,0})){g_playing=!g_playing;g_console.push_back(g_playing?"Play session started.":"Play session stopped.");}ImGui::End();
}
static LRESULT WINAPI WndProc(HWND h,UINT m,WPARAM w,LPARAM l){
 if(ImGui::GetCurrentContext()&&ImGui_ImplWin32_WndProcHandler(h,m,w,l))return true;
 switch(m){case WM_NCHITTEST:{LRESULT hit=DefWindowProcW(h,m,w,l);if(hit==HTCLIENT){POINT p{GET_X_LPARAM(l),GET_Y_LPARAM(l)};RECT r{};GetWindowRect(h,&r);int b=7;bool le=p.x<r.left+b,re=p.x>=r.right-b,te=p.y<r.top+b,be=p.y>=r.bottom-b;if(te&&le)return HTTOPLEFT;if(te&&re)return HTTOPRIGHT;if(be&&le)return HTBOTTOMLEFT;if(be&&re)return HTBOTTOMRIGHT;if(le)return HTLEFT;if(re)return HTRIGHT;if(te)return HTTOP;if(be)return HTBOTTOM;}return hit;}
 case WM_SIZE:if(g_device&&w!=SIZE_MINIMIZED){CleanupTarget();g_swapChain->ResizeBuffers(0,(UINT)LOWORD(l),(UINT)HIWORD(l),DXGI_FORMAT_UNKNOWN,0);CreateTarget();}return 0;
 case WM_SYSCOMMAND:if((w&0xfff0)==SC_KEYMENU)return 0;break;case WM_DESTROY:PostQuitMessage(0);return 0;}return DefWindowProc(h,m,w,l);
}
int APIENTRY WinMain(HINSTANCE instance,HINSTANCE,LPSTR,int){
 WNDCLASSEX wc{sizeof(WNDCLASSEX),CS_CLASSDC,WndProc,0,0,instance,nullptr,nullptr,nullptr,nullptr,_T("FoRgEStudioWindow"),nullptr};RegisterClassEx(&wc);
 g_window=CreateWindowExW(0,wc.lpszClassName,L"FoRgE Creator Studio",WS_POPUP|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,100,80,1500,940,nullptr,nullptr,instance,nullptr);
 HICON icon=LoadIconW(instance,MAKEINTRESOURCEW(IDI_APP_ICON));if(icon){SendMessageW(g_window,WM_SETICON,ICON_BIG,(LPARAM)icon);SendMessageW(g_window,WM_SETICON,ICON_SMALL,(LPARAM)icon);}MARGINS margins{1,1,1,1};DwmExtendFrameIntoClientArea(g_window,&margins);
 if(!CreateDevice(g_window)){CleanupTarget();if(g_swapChain)g_swapChain->Release();if(g_context)g_context->Release();if(g_device)g_device->Release();UnregisterClass(wc.lpszClassName,wc.hInstance);return 1;}
 ShowWindow(g_window,SW_SHOW);UpdateWindow(g_window);IMGUI_CHECKVERSION();ImGui::CreateContext();ImGui::GetIO().ConfigFlags|=ImGuiConfigFlags_NavEnableKeyboard;Theme();if(!LoadFonts())g_console.push_back("ERROR: FoRgE font pack missing from Resources/Fonts.");ImGui_ImplWin32_Init(g_window);ImGui_ImplDX11_Init(g_device,g_context);
 MSG msg{};while(msg.message!=WM_QUIT){if(PeekMessage(&msg,nullptr,0,0,PM_REMOVE)){TranslateMessage(&msg);DispatchMessage(&msg);continue;}ImGui_ImplDX11_NewFrame();ImGui_ImplWin32_NewFrame();ImGui::NewFrame();TitleBar();if(g_welcome)Welcome();else Editor();ImGui::Render();const float clear[]={.035f,.039f,.048f,1};g_context->OMSetRenderTargets(1,&g_target,nullptr);g_context->ClearRenderTargetView(g_target,clear);ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());g_swapChain->Present(1,0);}
 ImGui_ImplDX11_Shutdown();ImGui_ImplWin32_Shutdown();ImGui::DestroyContext();CleanupTarget();if(g_swapChain)g_swapChain->Release();if(g_context)g_context->Release();if(g_device)g_device->Release();DestroyWindow(g_window);UnregisterClass(wc.lpszClassName,wc.hInstance);return 0;
}
