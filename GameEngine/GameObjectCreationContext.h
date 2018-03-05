#pragma once
#include <string>
#include "MyVmath.h"

#define NUMBER_OFCREATABLE_GAME_OBJECTS 3

class GameObject;
class Transform;

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
   GameObject* OnGUI( GameObject& parent );

private:
   FunctionWithName m_CreationFunctions[ NUMBER_OFCREATABLE_GAME_OBJECTS ];
};
