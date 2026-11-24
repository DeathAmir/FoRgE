#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <fstream>
#include <string>
#include <utility>
namespace forge {
struct Block{int x;int z;int type;};
struct World{
 std::vector<Block> blocks;
 void Add(int x,int z,int type){for(auto& b:blocks)if(b.x==x&&b.z==z){b.type=type;return;}blocks.push_back({x,z,type});}
 void Remove(int x,int z){for(auto it=blocks.begin();it!=blocks.end();++it)if(it->x==x&&it->z==z){blocks.erase(it);return;}}
 bool Save(const std::string& path)const{std::ofstream out(path);if(!out)return false;out<<"FORGE_WORLD 1\n";for(const auto& b:blocks)out<<b.x<<' '<<b.z<<' '<<b.type<<'\n';return out.good();}
 bool Load(const std::string& path){std::ifstream in(path);std::string magic;int version;if(!(in>>magic>>version)||magic!="FORGE_WORLD"||version!=1)return false;std::vector<Block> next;Block b{};while(in>>b.x>>b.z>>b.type)if(b.type>=0&&b.type<=2)next.push_back(b);blocks=std::move(next);return true;}
};
struct CharacterRig{Model head{},torso{},leftArm{},rightArm{},leftLeg{},rightLeg{};Texture2D textures[6]{};bool loaded=false;};
inline Color BlockColor(int type){if(type==1)return Color{125,132,143,255};if(type==2)return Color{91,151,78,255};return Color{190,79,48,255};}
inline void DrawBlock(const Block& b){Vector3 p{(float)b.x,0.5f,(float)b.z};DrawCubeV(p,{1,1,1},BlockColor(b.type));DrawCubeWiresV(p,{1.005f,1.005f,1.005f},Color{35,35,39,255});}
inline CharacterRig LoadCharacter(){
 CharacterRig r;const char* paths[6]={"Resources/player_head.png","Resources/player_torso.png","Resources/player_arm.png","Resources/player_arm.png","Resources/player_leg.png","Resources/player_leg.png"};
 Model* models[6]={&r.head,&r.torso,&r.leftArm,&r.rightArm,&r.leftLeg,&r.rightLeg};
 for(int i=0;i<6;i++){r.textures[i]=LoadTexture(paths[i]);*models[i]=LoadModelFromMesh(GenMeshCube(1,1,1));if(r.textures[i].id)models[i]->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture=r.textures[i];}
 r.loaded=true;return r;
}
inline void DrawPart(Model model,Vector3 p,Vector3 size,float angle,Color tint=WHITE){DrawModelEx(model,p,{1,0,0},angle,size,tint);}
inline void DrawPlayer(const CharacterRig& r,Vector3 p,float phase=0.0f,float speed=0.0f){
 float amount=Clamp(speed,0.0f,1.0f);float swing=sinf(phase)*32.0f*amount;
 DrawPart(r.leftLeg,{p.x-0.22f,p.y+0.46f,p.z},{0.36f,0.92f,0.42f},swing);
 DrawPart(r.rightLeg,{p.x+0.22f,p.y+0.46f,p.z},{0.36f,0.92f,0.42f},-swing);
 DrawPart(r.torso,{p.x,p.y+1.38f,p.z},{0.88f,0.88f,0.48f},0);
 DrawPart(r.leftArm,{p.x-0.62f,p.y+1.39f,p.z},{0.30f,0.88f,0.34f},-swing);
 DrawPart(r.rightArm,{p.x+0.62f,p.y+1.39f,p.z},{0.30f,0.88f,0.34f},swing);
 DrawPart(r.head,{p.x,p.y+2.12f,p.z},{0.68f,0.68f,0.68f},0);
}
inline void UnloadCharacter(CharacterRig& r){Model* models[6]={&r.head,&r.torso,&r.leftArm,&r.rightArm,&r.leftLeg,&r.rightLeg};for(auto* m:models)if(m->meshCount)UnloadModel(*m);for(auto& t:r.textures)if(t.id)UnloadTexture(t);r.loaded=false;}
inline void DrawWorld(const World& world){for(const auto& b:world.blocks)DrawBlock(b);}
}