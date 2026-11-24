#include <raylib.h>
#include <raymath.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "forge/World.hpp"
#include <filesystem>
#include <string>
#include <cmath>
#include <cstdlib>
static int selectedType=0,selectedTool=0,fontIndex=4;
static Font brandFonts[7]{};
static const char* fontNames[7]={"Light 300","Regular 400","Medium 500","SemiBold 600","Bold 700","ExtraBold 800","Black 900"};
static const char* fontFiles[7]={"forge-300.ttf","forge-400.ttf","forge-500.ttf","forge-600.ttf","forge-700.ttf","forge-800.ttf","forge-900.ttf"};
static Camera3D camera{{12,12,12},{0,0,0},{0,1,0},45,CAMERA_PERSPECTIVE};
static void SetAppDirectory(const char* executable){std::error_code ec;auto p=std::filesystem::absolute(executable,ec);if(!ec)std::filesystem::current_path(p.parent_path(),ec);}
static Rectangle R(float x,float y,float w,float h){return Rectangle{x,y,w,h};}
static bool MouseInViewport(){Vector2 m=GetMousePosition();return m.x>184&&m.x<GetScreenWidth()-260&&m.y>62&&m.y<GetScreenHeight()-36;}
static bool GridHit(int* x,int* z){Ray ray=GetScreenToWorldRay(GetMousePosition(),camera);if(fabsf(ray.direction.y)<0.0001f)return false;float t=-ray.position.y/ray.direction.y;if(t<0)return false;Vector3 p=Vector3Add(ray.position,Vector3Scale(ray.direction,t));*x=(int)roundf(p.x);*z=(int)roundf(p.z);return true;}
int main(int argc,char** argv){
 if(argc>0&&argv[0])SetAppDirectory(argv[0]);SetConfigFlags(FLAG_WINDOW_RESIZABLE|FLAG_MSAA_4X_HINT);InitWindow(1500,920,"FoRgE Creator Studio");SetTargetFPS(120);
 Image icon=LoadImage("Resources/Icons/forge-256.png");if(icon.data){SetWindowIcon(icon);UnloadImage(icon);}
 GuiSetStyle(DEFAULT,TEXT_SIZE,16);GuiSetStyle(DEFAULT,BACKGROUND_COLOR,ColorToInt(Color{29,32,40,255}));GuiSetStyle(DEFAULT,BASE_COLOR_NORMAL,ColorToInt(Color{46,49,58,255}));GuiSetStyle(DEFAULT,BASE_COLOR_FOCUSED,ColorToInt(Color{132,57,42,255}));GuiSetStyle(DEFAULT,BORDER_COLOR_NORMAL,ColorToInt(Color{65,68,77,255}));GuiSetStyle(DEFAULT,TEXT_COLOR_NORMAL,ColorToInt(Color{225,226,230,255}));
 for(int i=0;i<7;i++)brandFonts[i]=LoadFontEx((std::string("Resources/Fonts/")+fontFiles[i]).c_str(),18,nullptr,0);
 if(brandFonts[fontIndex].texture.id)GuiSetFont(brandFonts[fontIndex]);
 Texture2D skin=LoadTexture("Resources/player_texture.png");Model playerCube=LoadModelFromMesh(GenMeshCube(1,1,1));if(skin.id)playerCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture=skin;
 forge::World world;if(!world.Load("scene.forge")){world.Add(3,2,0);world.Add(4,2,1);world.Add(3,3,2);}
 int hoverX=0,hoverZ=0;std::string status="Ready  |  Scene is live";
 while(!WindowShouldClose()){
  if(IsKeyDown(KEY_LEFT_CONTROL)&&IsKeyPressed(KEY_S))status=world.Save("scene.forge")?"Saved scene.forge":"Could not save scene";
  if(IsKeyDown(KEY_LEFT_ALT)&&IsKeyPressed(KEY_F4))break;
  if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)&&MouseInViewport()){Vector2 d=GetMouseDelta();camera.position.x-=d.x*0.035f;camera.position.z-=d.y*0.035f;}
  float wheel=GetMouseWheelMove();if(wheel!=0){Vector3 o=Vector3Subtract(camera.position,camera.target);o=Vector3Scale(o,1.0f-wheel*0.08f);camera.position=Vector3Add(camera.target,o);}
  if(MouseInViewport()&&GridHit(&hoverX,&hoverZ)){
   if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){if(selectedTool==0){world.Add(hoverX,hoverZ,selectedType);status="Block placed in live scene";}else{world.Remove(hoverX,hoverZ);status="Block removed";}}
   if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){world.Remove(hoverX,hoverZ);status="Block removed";}
  }
  BeginDrawing();ClearBackground(Color{18,21,28,255});BeginMode3D(camera);DrawWorld(world);DrawGrid(40,1.0f);DrawPlayer(playerCube,{0,0,0});
  if(MouseInViewport())DrawCubeWiresV({(float)hoverX,0.5f,(float)hoverZ},{1.04f,1.04f,1.04f},Color{245,142,90,255});EndMode3D();
  DrawRectangle(0,0,GetScreenWidth(),54,Color{25,28,36,255});DrawText("F",20,8,34,Color{222,91,61,255});DrawText("FoRgE",55,13,24,RAYWHITE);DrawText("CREATOR STUDIO  /  LIVE WORLD",145,18,14,Color{151,157,169,255});
  if(GuiButton(R(GetScreenWidth()-250,9,105,36),"SAVE  CTRL+S"))status=world.Save("scene.forge")?"Saved scene.forge":"Save failed";
  if(GuiButton(R(GetScreenWidth()-135,9,115,36),"PLAY TEST")){world.Save("scene.forge");std::system("start \"\" FoRgEPlayer.exe");status="Launching native C++ play test";}
  GuiPanel(R(0,54,178,(float)GetScreenHeight()-90),"TOOLBOX");GuiLabel(R(14,88,150,22),"BUILD TOOLS");
  if(GuiButton(R(14,116,150,36),selectedTool==0?"[x]  Place block":"Place block"))selectedTool=0;
  if(GuiButton(R(14,158,150,36),selectedTool==1?"[x]  Erase":"Erase"))selectedTool=1;
  GuiLabel(R(14,218,150,22),"MATERIALS");
  if(GuiButton(R(14,246,150,38),selectedType==0?"[x]  Copper":"Copper"))selectedType=0;
  if(GuiButton(R(14,290,150,38),selectedType==1?"[x]  Stone":"Stone"))selectedType=1;
  if(GuiButton(R(14,334,150,38),selectedType==2?"[x]  Grass":"Grass"))selectedType=2;
  GuiPanel(R((float)GetScreenWidth()-250,54,250,(float)GetScreenHeight()-90),"INSPECTOR");
  GuiLabel(R(GetScreenWidth()-232,88,210,24),"WORLD");GuiLabel(R(GetScreenWidth()-232,118,210,24),TextFormat("Blocks: %i",(int)world.blocks.size()));
  GuiLabel(R(GetScreenWidth()-232,148,210,24),TextFormat("Selected: %s",selectedType==0?"Copper":selectedType==1?"Stone":"Grass"));
  GuiLabel(R(GetScreenWidth()-232,198,210,24),"DEFAULT PLAYER RIG");GuiLabel(R(GetScreenWidth()-232,228,210,24),"Block humanoid / 6 parts");
  GuiLabel(R(GetScreenWidth()-232,258,210,24),skin.id?"Texture loaded":"Texture missing");GuiLabel(R(GetScreenWidth()-232,310,210,24),"BRAND TYPOGRAPHY");
  GuiLabel(R(GetScreenWidth()-232,340,210,24),fontNames[fontIndex]);
  if(GuiButton(R(GetScreenWidth()-232,370,98,32),"FONT -")){fontIndex=(fontIndex+6)%7;if(brandFonts[fontIndex].texture.id)GuiSetFont(brandFonts[fontIndex]);}
  if(GuiButton(R(GetScreenWidth()-126,370,104,32),"FONT +")){fontIndex=(fontIndex+1)%7;if(brandFonts[fontIndex].texture.id)GuiSetFont(brandFonts[fontIndex]);}
  GuiLabel(R(GetScreenWidth()-232,428,210,24),"CONTROLS");GuiLabel(R(GetScreenWidth()-232,458,210,24),"LMB  Place selected block");GuiLabel(R(GetScreenWidth()-232,484,210,24),"RMB  Remove block");GuiLabel(R(GetScreenWidth()-232,510,210,24),"Wheel  Zoom");GuiLabel(R(GetScreenWidth()-232,536,210,24),"MMB  Pan view");
  DrawRectangle(0,GetScreenHeight()-36,GetScreenWidth(),36,Color{25,28,36,255});DrawText(status.c_str(),14,GetScreenHeight()-25,14,Color{181,187,198,255});DrawText(TextFormat("FPS %i  |  C++ ENGINE",GetFPS()),GetScreenWidth()-220,GetScreenHeight()-25,14,Color{130,137,149,255});EndDrawing();
 }
 UnloadModel(playerCube);if(skin.id)UnloadTexture(skin);for(auto& f:brandFonts)if(f.texture.id)UnloadFont(f);CloseWindow();return 0;
}