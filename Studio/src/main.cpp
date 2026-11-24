#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>
#include <WebView2.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
using Microsoft::WRL::ComPtr;
static HWND g_window{};
static ComPtr<ICoreWebView2Controller> g_controller;
static ComPtr<ICoreWebView2> g_webview;
static std::filesystem::path AppDir(){wchar_t p[MAX_PATH]{};GetModuleFileNameW(nullptr,p,MAX_PATH);return std::filesystem::path(p).parent_path();}
static void ResizeView(){if(!g_controller)return;RECT r{};GetClientRect(g_window,&r);g_controller->put_Bounds(r);}
static void LaunchPlayer(){
 auto exe=AppDir()/L"FoRgEPlayer.exe";
 if(!std::filesystem::exists(exe)){MessageBoxW(g_window,L"FoRgEPlayer.exe is missing.",L"FoRgE Studio",MB_ICONERROR);return;}
 std::wstring cmd=L"\""+exe.wstring()+L"\"";STARTUPINFOW si{sizeof(si)};PROCESS_INFORMATION pi{};
 if(CreateProcessW(exe.c_str(),cmd.data(),nullptr,nullptr,FALSE,0,nullptr,AppDir().c_str(),&si,&pi)){CloseHandle(pi.hThread);CloseHandle(pi.hProcess);}
}
static std::string Narrow(const wchar_t* value){
 if(!value)return {};int n=WideCharToMultiByte(CP_UTF8,0,value,-1,nullptr,0,nullptr,nullptr);
 std::string out(n? n:0,'\0');if(n>0){WideCharToMultiByte(CP_UTF8,0,value,-1,out.data(),n,nullptr,nullptr);out.pop_back();}return out;
}
static void SaveScene(const std::string& payload){
 std::ofstream out(AppDir()/L"scene.forge",std::ios::trunc);if(!out)return;
 out<<"FORGE_WORLD 1\n";std::stringstream rows(payload);std::string row;
 while(std::getline(rows,row,';')){std::replace(row.begin(),row.end(),',',' ');if(!row.empty())out<<row<<'\n';}
}
static HRESULT OnMessage(ICoreWebView2*,ICoreWebView2WebMessageReceivedEventArgs* args){
 LPWSTR raw=nullptr;if(FAILED(args->TryGetWebMessageAsString(&raw))||!raw)return S_OK;
 std::wstring message(raw);CoTaskMemFree(raw);
 if(message==L"play")LaunchPlayer();else if(message.rfind(L"save|",0)==0)SaveScene(Narrow(message.c_str()+5));
 return S_OK;
}
static void CreateView(){
 CreateCoreWebView2EnvironmentWithOptions(nullptr,nullptr,nullptr,Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
 [](HRESULT hr,ICoreWebView2Environment* env)->HRESULT{
  if(FAILED(hr)||!env)return hr;
  return env->CreateCoreWebView2Controller(g_window,Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
   [](HRESULT result,ICoreWebView2Controller* controller)->HRESULT{
    if(FAILED(result)||!controller)return result;g_controller=controller;g_controller->get_CoreWebView2(&g_webview);ResizeView();
    ComPtr<ICoreWebView2_3> webview3;
    if(SUCCEEDED(g_webview.As(&webview3))){auto ui=(AppDir()/L"StudioUI").wstring();webview3->SetVirtualHostNameToFolderMapping(L"forge-studio.local",ui.c_str(),COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);}
    EventRegistrationToken token{};g_webview->add_WebMessageReceived(Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(OnMessage).Get(),&token);
    return g_webview->Navigate(L"https://forge-studio.local/index.html");
   }).Get());
 }).Get());
}
static LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM w,LPARAM l){
 if(msg==WM_SIZE){ResizeView();return 0;}if(msg==WM_DESTROY){if(g_controller)g_controller->Close();PostQuitMessage(0);return 0;}return DefWindowProcW(hwnd,msg,w,l);
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE,PWSTR,int show){
 CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);WNDCLASSW wc{};wc.lpfnWndProc=WndProc;wc.hInstance=instance;wc.hCursor=LoadCursor(nullptr,IDC_ARROW);wc.lpszClassName=L"FoRgECreatorStudio";RegisterClassW(&wc);
 g_window=CreateWindowExW(0,wc.lpszClassName,L"FoRgE Creator Studio",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,1540,960,nullptr,nullptr,instance,nullptr);
 if(!g_window){CoUninitialize();return 1;}
 auto icon=AppDir()/L"Resources"/L"Icons"/L"forge.ico";HICON h=(HICON)LoadImageW(nullptr,icon.c_str(),IMAGE_ICON,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE);
 if(h){SendMessageW(g_window,WM_SETICON,ICON_BIG,(LPARAM)h);SendMessageW(g_window,WM_SETICON,ICON_SMALL,(LPARAM)h);}
 ShowWindow(g_window,show);CreateView();MSG msg{};while(GetMessageW(&msg,nullptr,0,0)>0){TranslateMessage(&msg);DispatchMessageW(&msg);}
 g_webview.Reset();g_controller.Reset();CoUninitialize();return 0;
}