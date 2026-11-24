#pragma once
#include <raylib.h>
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
inline Color BlockColor(int type){if(type==1)return Color{125,132,143,255};if(type==2)return Color{91,151,78,255};return Color{190,79,48,255};}
inline void DrawBlock(const Block& b){Vector3 p{(float)b.x,0.5f,(float)b.z};DrawCubeV(p,{1,1,1},BlockColor(b.type));DrawCubeWiresV(p,{1.005f,1.005f,1.005f},Color{35,35,39,255});}
inline void DrawPlayer(Model cube,Vector3 p,Color tint=WHITE){
 DrawModelEx(cube,{p.x,p.y+0.41f,p.z-0.22f},{0,1,0},0,{0.34f,0.82f,0.36f},tint);
 DrawModelEx(cube,{p.x,p.y+0.41f,p.z+0.22f},{0,1,0},0,{0.34f,0.82f,0.36f},tint);
 DrawModelEx(cube,{p.x,p.y+1.35f,p.z},{0,1,0},0,{0.86f,0.78f,0.42f},tint);
 DrawModelEx(cube,{p.x,p.y+2.05f,p.z},{0,1,0},0,{0.66f,0.66f,0.66f},tint);
 DrawModelEx(cube,{p.x-0.55f,p.y+1.35f,p.z},{0,1,0},0,{0.30f,0.82f,0.34f},tint);
 DrawModelEx(cube,{p.x+0.55f,p.y+1.35f,p.z},{0,1,0},0,{0.30f,0.82f,0.34f},tint);
}
inline void DrawWorld(const World& world){for(const auto& b:world.blocks)DrawBlock(b);}
}