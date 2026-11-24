#include <raylib.h>
#include <raymath.h>
#include <filesystem>
#include <windows.h>
#include "forge/World.hpp"
static void SetAppDirectory(){wchar_t p[MAX_PATH]{};GetModuleFileNameW(nullptr,p,MAX_PATH);SetCurrentDirectoryW(std::filesystem::path(p).parent_path().c_str());}
int main(){
 SetAppDirectory();SetConfigFlags(FLAG_WINDOW_RESIZABLE|FLAG_MSAA_4X_HINT);InitWindow(1280,720,"FoRgE Player");SetTargetFPS(144);
 Image icon=LoadImage("Resources/Icons/forge-256.png");if(icon.data){SetWindowIcon(icon);UnloadImage(icon);}
 Texture2D skin=LoadTexture("Resources/player_texture.png");Model cube=LoadModelFromMesh(GenMeshCube(1,1,1));if(skin.id)cube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture=skin;
 forge::World world;if(!world.Load("scene.forge")){for(int x=-8;x<=8;++x)for(int z=-8;z<=8;++z)world.Add(x,z,2);world.Add(3,2,0);world.Add(4,2,1);}
 Vector3 player{0,0,0},velocity{0,0,0};Camera3D camera{};camera.up={0,1,0};camera.fovy=48;camera.projection=CAMERA_PERSPECTIVE;
 while(!WindowShouldClose()){
  float dt=GetFrameTime();Vector3 move{0,0,0};if(IsKeyDown(KEY_W))move.z-=1;if(IsKeyDown(KEY_S))move.z+=1;if(IsKeyDown(KEY_A))move.x-=1;if(IsKeyDown(KEY_D))move.x+=1;
  if(Vector3Length(move)>0)move=Vector3Normalize(move);player.x+=move.x*5.0f*dt;player.z+=move.z*5.0f*dt;
  if(IsKeyPressed(KEY_SPACE)&&player.y<=0.001f)velocity.y=7.0f;velocity.y-=18.0f*dt;player.y+=velocity.y*dt;if(player.y<0){player.y=0;velocity.y=0;}
  camera.target={player.x,player.y+1.1f,player.z};camera.position={player.x+7.5f,player.y+6.0f,player.z+8.5f};
  BeginDrawing();ClearBackground(Color{18,21,28,255});BeginMode3D(camera);DrawWorld(world);DrawGrid(32,1.0f);DrawPlayer(cube,player);EndMode3D();
  DrawRectangle(0,0,GetScreenWidth(),48,Color{24,27,35,255});DrawText("FoRgE PLAYER",20,15,20,Color{225,105,75,255});DrawText("WASD MOVE   SPACE JUMP",GetScreenWidth()-280,17,16,GRAY);DrawText("Native C++ runtime / local scene",20,GetScreenHeight()-28,14,Color{145,151,163,255});EndDrawing();
 }
 UnloadModel(cube);if(skin.id)UnloadTexture(skin);CloseWindow();return 0;
}