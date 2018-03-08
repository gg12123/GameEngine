#pragma once
#include <string>
#include "MyVmath.h"

#define NUMBER_OF_CREATABLE_GAME_OBJECTS 3

class GameObject;
class Transform;
class World;

typedef GameObject&(*GOCreationFunctionPtr)(vmath::vec3 pos, vmath::mat4 rot, Transform& parent);

struct FunctionWithName
{
   std::string Name;
   GOCreationFunctionPtr Function;

   void Init( std::string n, GOCreationFunctionPtr f )
   {
      Name = n;
      Function = f;
   }
};

class GameObjectCreationContext
{
public:
   GameObjectCreationContext();
   void OnGUI( World& world, GameObject& parent );

private:
   FunctionWithName m_CreationFunctions[ NUMBER_OF_CREATABLE_GAME_OBJECTS ];
};

void AddComponentOnGUI( World& world, GameObject& active );
