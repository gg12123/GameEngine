#pragma once
#include <string>
#include <vector>
#include "MyVmath.h"

#define NUMBER_OF_CREATABLE_GAME_OBJECTS 3

class GameObject;
class Transform;
class Editor;

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
   void OnGUI( Editor& editor, GameObject& parent );

private:
   FunctionWithName m_CreationFunctions[ NUMBER_OF_CREATABLE_GAME_OBJECTS ];
};

class PrefabSpawnContext
{
public:
   void OnGUI( Editor& editor, GameObject& parent );
private:
   std::vector<std::string> m_AvailablePrefabs;
};

#define MAX_GO_NAME_SIZE 50

class GameObjectRenamer
{
public:
   void OnGUI( GameObject& active );
private:
   char m_Buffer[ MAX_GO_NAME_SIZE ];
};

void AddComponentOnGUI( Editor& editor, GameObject& active );
