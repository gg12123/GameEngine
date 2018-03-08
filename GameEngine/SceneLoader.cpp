#include "SceneLoader.h"
#include "Utils.h"
#include "World.h"
#include "Path.h"
#include <iostream>
#include <fstream>

SceneLoader::SceneLoader()
{
   m_World = nullptr;
}

void SceneLoader::LoadCompletelyNewScene( std::string name )
{
   m_World->ClearAll();

   std::vector<GameObject*> objects;
   GameObject& root = HierarchyForNewProject( objects );
   CallAwake( *m_World, root, objects );

   m_ActiveSceneName = name;
}

void SceneLoader::LoadStartUpScene()
{
   if (!LoadScene( "StartScene.scene" ))
   {
      LoadCompletelyNewScene( "StartScene.scene" );
   }
}

void SceneLoader::PendLoadScene( std::string name )
{
   m_PendingSceneName = name;
   m_LoadPending = true;
}

void SceneLoader::Update()
{
   if (m_LoadPending)
   {
      if (!LoadScene( m_PendingSceneName ))
      {
         throw std::exception( "Unable to load pending scene" );
      }

      m_LoadPending = false;
   }
}

bool SceneLoader::LoadScene( std::string name )
{
   bool success = false;
   std::string path = Path::Instance().GetScenePath( name );

   std::ifstream stream;

   stream.open( path, std::ios::binary );

   if (stream.is_open())
   {
      success = true;
      std::vector<GameObject*> gameObjects;

      GameObject& root = DeSerializeHierarchy( stream, gameObjects );

      m_World->ClearAll();
      CallAwake( *m_World, root, gameObjects );

      stream.close();

      m_ActiveSceneName = name;
   }

   return success;
}