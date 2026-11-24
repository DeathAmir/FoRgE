#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>
#include <WebView2.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
using Microsoft::WRL::ComPtr;
static ComPtr<ICoreWebView2Controller> g_controller;
static ComPtr<ICoreWebView2> g_webview;
static HWND g_window=nullptr;
static std::filesystem::path AppDir(){wchar_t p[MAX_PATH]{};GetModuleFileNameW(nullptr,p,MAX_PATH);return std::filesystem::path(p).parent_path();}
static std::string Base64File(const std::filesystem::path& path){
 std::ifstream in(path,std::ios::binary);if(!in)return {};std::vector<unsigned char> data((std::istreambuf_iterator<char>(in)),{});
 static constexpr char alphabet[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 std::string out;out.reserve((data.size()+2)/3*4);
 for(size_t i=0;i<data.size();i+=3){unsigned a=data[i],b=i+1<data.size()?data[i+1]:0,c=i+2<data.size()?data[i+2]:0;unsigned v=(a<<16)|(b<<8)|c;out.push_back(alphabet[(v>>18)&63]);out.push_back(alphabet[(v>>12)&63]);out.push_back(i+1<data.size()?alphabet[(v>>6)&63]:'=');out.push_back(i+2<data.size()?alphabet[v&63]:'=');}
 return out;
}
static void Launch(const wchar_t* exe){
 auto path=AppDir()/exe;if(!std::filesystem::exists(path)){MessageBoxW(g_window,(L"Missing application: "+path.wstring()).c_str(),L"FoRgE Client",MB_ICONERROR);return;}
 std::wstring command=L"\""+path.wstring()+L"\"";STARTUPINFOW si{sizeof(si)};PROCESS_INFORMATION pi{};
 if(CreateProcessW(path.c_str(),command.data(),nullptr,nullptr,FALSE,0,nullptr,AppDir().c_str(),&si,&pi)){CloseHandle(pi.hThread);CloseHandle(pi.hProcess);}else MessageBoxW(g_window,L"Could not start the selected FoRgE application.",L"FoRgE Client",MB_ICONERROR);
}
static void ResizeWebView(){if(!g_controller)return;RECT r{};GetClientRect(g_window,&r);g_controller->put_Bounds(r);}
static void CreateWebView(){
 CreateCoreWebView2EnvironmentWithOptions(nullptr,nullptr,nullptr,Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
 [](HRESULT result,ICoreWebView2Environment* env)->HRESULT{
  if(FAILED(result)||!env)return result;
  return env->CreateCoreWebView2Controller(g_window,Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
   [](HRESULT hr,ICoreWebView2Controller* controller)->HRESULT{
    if(FAILED(hr)||!controller)return hr;g_controller=controller;g_controller->get_CoreWebView2(&g_webview);ResizeWebView();EventRegistrationToken token{};
    g_webview->add_WebMessageReceived(Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
     [](ICoreWebView2*,ICoreWebView2WebMessageReceivedEventArgs* args)->HRESULT{
      LPWSTR message=nullptr;if(SUCCEEDED(args->TryGetWebMessageAsString(&message))&&message){if(wcscmp(message,L"play")==0)Launch(L"FoRgEPlayer.exe");else if(wcscmp(message,L"studio")==0)Launch(L"FoRgEStudio.exe");CoTaskMemFree(message);}return S_OK;
     }).Get(),&token);
    std::wstring html=LR"HTML(
<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><style>
*{box-sizing:border-box}body{margin:0;background:#12151c;color:#ececf0;font:16px 'Segoe UI',sans-serif;overflow:hidden}
.top{height:64px;background:#191d26;border-bottom:1px solid #303541;display:flex;align-items:center;padding:0 34px;gap:14px}
.mark{font-size:30px;font-weight:900;color:#d85b3c}.brand{font-size:20px;font-weight:700;letter-spacing:2px}.tag{color:#8d94a2;font-size:12px;letter-spacing:2px;margin-left:8px}
main{max-width:1120px;margin:8vh auto;padding:0 30px}.eyebrow{color:#df7958;letter-spacing:4px;font-size:12px;font-weight:700}
h1{font-size:58px;line-height:1.02;margin:18px 0 14px;max-width:650px}p{color:#a5abb7;line-height:1.7;max-width:570px}
.actions{display:flex;gap:12px;margin-top:34px}.btn{border:1px solid #454b58;border-radius:7px;background:#252a35;color:#f3f3f5;padding:15px 24px;font-size:14px;font-weight:700;cursor:pointer}
.primary{background:#bd4c35;border-color:#d46143}.btn:hover{filter:brightness(1.15)}
.panel{margin-top:64px;border:1px solid #343a46;background:#191d26;border-radius:10px;padding:22px;display:flex;justify-content:space-between;align-items:center}
.small{font-size:12px;color:#858d9c;letter-spacing:2px}.status{color:#b5bdc9;font-size:13px}
</style></head><body><header class="top"><span class="mark">F</span><span class="brand">FoRgE</span><span class="tag">GAME PLATFORM</span></header>
<main><div class="eyebrow">CREATE · PLAY · SHARE</div><h1>Your next world starts here.</h1><p>Build block-based worlds in Creator Studio, test them in the native C++ runtime, and keep your projects together in one desktop client.</p>
<div class="actions"><button class="btn primary" onclick="send('play')">PLAY FoRgE</button><button class="btn" onclick="send('studio')">OPEN CREATOR STUDIO</button></div>
<section class="panel"><div><div class="small">FoRgE DESKTOP CLIENT</div><div style="margin-top:8px;font-weight:600">Native runtime · Local world preview</div></div><div class="status">WEBVIEW2 INTERFACE / C++ HOST</div></section></main>
<script>function send(action){window.chrome.webview.postMessage(action)}</script></body></html>)HTML";
    const auto font=Base64File(AppDir()/L"Resources"/L"Fonts"/L"forge-400.ttf");
    if(!font.empty()){std::wstring css=L"@font-face{font-family:FoRgE;src:url(data:font/ttf;base64,"+std::wstring(font.begin(),font.end())+L")}body{font-family:FoRgE,'Segoe UI',sans-serif}";auto at=html.find(L"</style>");if(at!=std::wstring::npos)html.insert(at,css);}
    return g_webview->NavigateToString(html.c_str());
   }).Get());
 }).Get());
}
static LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM w,LPARAM l){if(msg==WM_SIZE){ResizeWebView();return 0;}if(msg==WM_DESTROY){if(g_controller)g_controller->Close();PostQuitMessage(0);return 0;}return DefWindowProcW(hwnd,msg,w,l);}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE,PWSTR,int show){
 CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);WNDCLASSW wc{};wc.lpfnWndProc=WndProc;wc.hInstance=instance;wc.hCursor=LoadCursor(nullptr,IDC_ARROW);wc.lpszClassName=L"FoRgEClientWindow";RegisterClassW(&wc);
 g_window=CreateWindowExW(0,wc.lpszClassName,L"FoRgE Client",WS_OVERLAPPEDWINDOW|WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,1180,780,nullptr,nullptr,instance,nullptr);
 if(!g_window){CoUninitialize();return 1;}
 auto iconPath=AppDir()/L"Resources"/L"Icons"/L"forge.ico";HICON icon=(HICON)LoadImageW(nullptr,iconPath.c_str(),IMAGE_ICON,0,0,LR_LOADFROMFILE|LR_DEFAULTSIZE);if(icon){SendMessageW(g_window,WM_SETICON,ICON_BIG,(LPARAM)icon);SendMessageW(g_window,WM_SETICON,ICON_SMALL,(LPARAM)icon);}
 ShowWindow(g_window,show);CreateWebView();MSG msg{};while(GetMessageW(&msg,nullptr,0,0)>0){TranslateMessage(&msg);DispatchMessageW(&msg);}
 g_webview.Reset();g_controller.Reset();CoUninitialize();return 0;
}