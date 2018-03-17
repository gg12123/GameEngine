#pragma once
#include <string>
#include <vector>
#include "MyVmath.h"

#define NUMBER_OF_CREATABLE_GAME_OBJECTS 4

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

class PrefabSpawnSaveContext
{
public:
   void OnGUI( Editor& editor, GameObject& active );
private:
   void SaveAsPrefab( Editor& editor, GameObject& active );
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

#define MAX_SCENE_NAME_SIZE 50
class EditorSceneManagement
{
public:
   void OnGUI( Editor& editor );
private:
   void SaveActiveScene( Editor& editor );

   char m_Buffer[ MAX_SCENE_NAME_SIZE ];
   std::vector<std::string> m_AvailableScenes;
};

void AddComponentOnGUI( Editor& editor, GameObject& active );
