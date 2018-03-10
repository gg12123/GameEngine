#include "SceneLoader.h"
#include "Utils.h"
#include "World.h"
#include "Path.h"
#include "Editor.h"
#include <iostream>
#include <fstream>

std::string SceneLoader::GetActiveSceneName()
{
   return m_ActiveSceneName;
}

void SceneLoader::LoadCompletelyNewScene( std::string name )
{
   ClearAll();

   std::vector<GameObject*> objects;
   GameObject& root = HierarchyForNewProject( objects );
   CallAwake( root, objects );

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

      ClearAll();
      CallAwake( root, gameObjects );

      stream.close();

      m_ActiveSceneName = name;
   }

   return success;
}

EditModeSceneLoader::EditModeSceneLoader()
{
   m_Editor = nullptr;
}

void EditModeSceneLoader::Init( Editor& editor )
{
   m_Editor = &editor;
}

void EditModeSceneLoader::CallAwake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects )
{
   m_Editor->InvokeEvent( eSceneLoad );
   m_Editor->GetWorld().EditAwake( *m_Editor, rootGameObject, gameObjects );
}

void EditModeSceneLoader::ClearAll()
{
   m_Editor->GetWorld().ClearAll();
}

PlayModeSceneLoader::PlayModeSceneLoader()
{
   m_World = nullptr;
}

void PlayModeSceneLoader::Init( World& world )
{
   m_World = &world;
}

void PlayModeSceneLoader::CallAwake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects )
{
   m_World->Awake( rootGameObject, gameObjects );
}

void PlayModeSceneLoader::ClearAll()
{
   m_World->ClearAll();
}