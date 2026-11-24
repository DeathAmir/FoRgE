#include <raylib.h>
#include <raymath.h>
#include <filesystem>
#include <memory>
#include <vector>
#include <cmath>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "forge/World.hpp"
static void SetAppDirectory(const char* executable){std::error_code ec;auto p=std::filesystem::absolute(executable,ec);if(!ec)std::filesystem::current_path(p.parent_path(),ec);}
struct PhysicsScene{
 btDefaultCollisionConfiguration config;
 btCollisionDispatcher dispatcher{&config};
 btDbvtBroadphase broadphase;
 btSequentialImpulseConstraintSolver solver;
 btDiscreteDynamicsWorld world{&dispatcher,&broadphase,&solver,&config};
 std::vector<std::unique_ptr<btCollisionShape>> shapes;
 std::vector<std::unique_ptr<btMotionState>> motionStates;
 std::vector<std::unique_ptr<btRigidBody>> bodies;
 std::unique_ptr<btGhostPairCallback> ghostCallback;
 std::unique_ptr<btPairCachingGhostObject> ghost;
 std::unique_ptr<btCapsuleShape> capsule;
 std::unique_ptr<btKinematicCharacterController> character;
 PhysicsScene(){world.setGravity(btVector3(0,-20,0));auto ground=std::make_unique<btStaticPlaneShape>(btVector3(0,1,0),0);addStatic(std::move(ground),btVector3(0,0,0));}
 ~PhysicsScene(){if(character)world.removeAction(character.get());if(ghost)world.removeCollisionObject(ghost.get());for(auto& body:bodies)world.removeRigidBody(body.get());}
 void addStatic(std::unique_ptr<btCollisionShape> shape,const btVector3& position){
  btTransform transform;transform.setIdentity();transform.setOrigin(position);
  auto motion=std::make_unique<btDefaultMotionState>(transform);
  btRigidBody::btRigidBodyConstructionInfo info(0,motion.get(),shape.get());
  auto body=std::make_unique<btRigidBody>(info);world.addRigidBody(body.get());
  shapes.push_back(std::move(shape));motionStates.push_back(std::move(motion));bodies.push_back(std::move(body));
 }
 void addBlocks(const forge::World& scene){for(const auto& b:scene.blocks)addStatic(std::make_unique<btBoxShape>(btVector3(.5,.5,.5)),btVector3((btScalar)b.x,.5,(btScalar)b.z));}
 void createCharacter(){
  capsule=std::make_unique<btCapsuleShape>(btScalar(.38),btScalar(1.70));
  ghostCallback=std::make_unique<btGhostPairCallback>();world.getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(ghostCallback.get());
  ghost=std::make_unique<btPairCachingGhostObject>();btTransform t;t.setIdentity();t.setOrigin(btVector3(0,2.23,0));
  ghost->setWorldTransform(t);ghost->setCollisionShape(capsule.get());ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
  world.addCollisionObject(ghost.get(),btBroadphaseProxy::CharacterFilter,btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
  character=std::make_unique<btKinematicCharacterController>(ghost.get(),capsule.get(),btScalar(.32),1);
  character->setGravity(btVector3(0,-20,0));character->setJumpSpeed(7.4f);character->setFallSpeed(35);character->setMaxSlope(btRadians(48));world.addAction(character.get());
 }
 Vector3 position()const{const auto p=ghost->getWorldTransform().getOrigin();return {(float)p.x(),(float)(p.y()-1.23f),(float)p.z()};}
 void step(float dt){world.stepSimulation(dt,8,1.0f/120.0f);}
};
int main(int argc,char** argv){
 if(argc>0&&argv[0])SetAppDirectory(argv[0]);SetConfigFlags(FLAG_WINDOW_RESIZABLE|FLAG_MSAA_4X_HINT);InitWindow(1280,720,"FoRgE Player");SetTargetFPS(144);
 Image icon=LoadImage("Resources/Icons/forge-256.png");if(icon.data){SetWindowIcon(icon);UnloadImage(icon);}
 Font brand=LoadFontEx("Resources/Fonts/forge-600.ttf",22,nullptr,0);forge::CharacterRig rig=forge::LoadCharacter();
 forge::World world;if(!world.Load("scene.forge")){for(int x=-8;x<=8;++x)for(int z=-8;z<=8;++z)world.Add(x,z,2);world.Add(3,2,0);world.Add(4,2,1);}
 PhysicsScene physics;physics.addBlocks(world);physics.createCharacter();Camera3D camera{};camera.up={0,1,0};camera.fovy=48;camera.projection=CAMERA_PERSPECTIVE;float walkPhase=0;
 while(!WindowShouldClose()){
  float dt=Clamp(GetFrameTime(),0.0f,1.0f/30.0f);Vector3 move{};
  if(IsKeyDown(KEY_W))move.z-=1;if(IsKeyDown(KEY_S))move.z+=1;if(IsKeyDown(KEY_A))move.x-=1;if(IsKeyDown(KEY_D))move.x+=1;
  float moveSpeed=Vector3Length(move);if(moveSpeed>0){move=Vector3Normalize(move);walkPhase+=dt*10.5f;}
  physics.character->setWalkDirection(btVector3(move.x*5.2f*dt,0,move.z*5.2f*dt));
  if(IsKeyPressed(KEY_SPACE)&&physics.character->onGround())physics.character->jump();
  physics.step(dt);Vector3 player=physics.position();camera.target={player.x,player.y+1.15f,player.z};camera.position={player.x+7.2f,player.y+5.2f,player.z+8.0f};
  BeginDrawing();ClearBackground(Color{25,29,31,255});BeginMode3D(camera);DrawWorld(world);DrawGrid(40,1.0f);forge::DrawPlayer(rig,player,walkPhase,moveSpeed);EndMode3D();
  DrawRectangle(0,0,GetScreenWidth(),48,Color{25,29,31,245});
  if(brand.texture.id)DrawTextEx(brand,"FoRgE PLAYER",{18,12},22,0,Color{226,111,78,255});else DrawText("FoRgE PLAYER",18,14,20,Color{226,111,78,255});
  DrawText("WASD MOVE   SPACE JUMP",GetScreenWidth()-280,17,14,Color{205,210,205,255});
  DrawText("BULLET CHARACTER CONTROLLER  /  NATIVE C++",20,GetScreenHeight()-27,12,Color{145,154,151,255});EndDrawing();
 }
 forge::UnloadCharacter(rig);if(brand.texture.id)UnloadFont(brand);CloseWindow();return 0;
}