#include <iostream>
#include <fstream>
#include "EditorUtils.h"
#include "GameObject.h"
#include "Transform.h"
#include "Utils.h"
#include "ImGUI/imgui.h"
#include "ComponentCreator.h"
#include "Editor.h"
#include "Path.h"
#include "World.h"
#include "SceneLoader.h"

GameObjectCreationContext::GameObjectCreationContext()
{
   m_CreationFunctions[ 0 ].Init( "Light", CreateLightGameObject );
   m_CreationFunctions[ 1 ].Init( "Camera", CreateCameraGameObject );
   m_CreationFunctions[ 2 ].Init( "Cube", CreateCubeGameObject );
   m_CreationFunctions[ 3 ].Init( "Empty", CreateEmptyGameObject );
}

void GameObjectCreationContext::OnGUI( Editor& editor, GameObject& parent )
{
   if (ImGui::BeginMenu( "New game object" ))
   {
      for (int i = 0; i < NUMBER_OF_CREATABLE_GAME_OBJECTS; i++)
      {
         if (ImGui::Selectable( m_CreationFunctions[ i ].Name.c_str() ))
         {
            GameObject& newObj = m_CreationFunctions[ i ].Function( vmath::vec3( 0.0f, 0.0f, 0.0f ), vmath::mat4().identity(), parent.GetTransform() );

            std::vector<GameObject*> hierarchy;
            hierarchy.push_back( &newObj );

            editor.GetWorld().EditAwakeHierarchy( editor, hierarchy );
         }
      }

      ImGui::EndMenu();
   }
}

void PrefabSpawnSaveContext::OnGUI( Editor& editor, GameObject& active )
{
   if (ImGui::BeginMenu( "Spawn prefab" ))
   {
      m_AvailablePrefabs.clear();
      GetFileNamesInDirectory( Path::Instance().GetPrefabPath( "" ), m_AvailablePrefabs );

      for (auto it = m_AvailablePrefabs.begin(); it != m_AvailablePrefabs.end(); it++)
      {
         if (ImGui::Selectable( it->c_str() ))
         {
            PrefabField prefabField;
            prefabField.SetValue( *it );
            prefabField.InstantiateEdit( editor, active.GetTransform() );
            break;
         }
      }

      ImGui::EndMenu();
   }

   if (ImGui::BeginMenu( "Apply to prefab" ))
   {
      ImGui::Text( "Are you sure?" );
      ImGui::SameLine();
      if (ImGui::SmallButton( "Yes" ))
      {
         SaveAsPrefab( editor, active );
      }
      ImGui::EndMenu();
   }
}

void PrefabSpawnSaveContext::SaveAsPrefab( Editor& editor, GameObject& active )
{
   std::string path = Path::Instance().GetPrefabPath( active.GetName() + ".prefab" );

   std::ofstream stream;
   stream.open( path, std::ios::binary );

   if (!stream.is_open())
   {
      throw std::exception( "unable to save prefab" );
   }

   SerializeHierarchy( active, stream );

   stream.close();
}

void GameObjectRenamer::OnGUI( GameObject& active )
{
   if (ImGui::Button( "Rename" ))
   {
      ImGui::OpenPopup( "RenameGOPopup" );

      std::string currName = active.GetName();

      if (currName.size() > MAX_GO_NAME_SIZE)
      {
         throw std::exception( "GO name size is too big" );
      }

      CopyStringToBuffer( m_Buffer, currName );
   }

   if (ImGui::BeginPopupModal( "RenameGOPopup", NULL, ImGuiWindowFlags_AlwaysAutoResize ))
   {
      ImGui::InputText( "Name", m_Buffer, MAX_GO_NAME_SIZE );

      if (ImGui::Button( "OK" ))
      {
         active.SetName( m_Buffer );
         ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
   }
}

void EditorSceneManagement::OnGUI( Editor& editor )
{
   if (ImGui::BeginMenu( "Scenes" ))
   {
      if (ImGui::MenuItem( "Save current" ))
      {
         SaveActiveScene( editor );
      }

      if (ImGui::BeginMenu( "Load scene" ))
      {
         m_AvailableScenes.clear();
         GetFileNamesInDirectory( Path::Instance().GetScenePath( "" ), m_AvailableScenes );

         for (auto it = m_AvailableScenes.begin(); it != m_AvailableScenes.end(); it++)
         {
            if (ImGui::Selectable( it->c_str() ))
            {
               editor.GetWorld().GetSceneLoader().LoadScene( *it );
               break;
            }
         }

         ImGui::EndMenu();
      }

      if (ImGui::MenuItem( "Start new scene" ))
      {
         ImGui::OpenPopup( "NewScenePopup" );
         CopyStringToBuffer( m_Buffer, "Write name here" );
      }

      if (ImGui::BeginPopupModal( "NewScenePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize ))
      {
         ImGui::InputText( "Name", m_Buffer, MAX_SCENE_NAME_SIZE );

         if (ImGui::Button( "OK" ))
         {
            std::string selectedName = m_Buffer;
            selectedName.append( ".scene" );

            editor.GetWorld().GetSceneLoader().LoadCompletelyNewScene( selectedName );
            ImGui::CloseCurrentPopup();
         }

         ImGui::EndPopup();
      }

      ImGui::EndMenu();
   }
}

void EditorSceneManagement::SaveActiveScene( Editor& editor )
{
   std::string path = Path::Instance().GetScenePath( editor.GetWorld().GetSceneLoader().GetActiveSceneName() );

   std::ofstream stream;
   stream.open( path, std::ios::binary );

   if (!stream.is_open())
   {
      throw std::exception( "unable to save scene" );
   }

   SerializeHierarchy( editor.GetWorld().GetRootTransform().GetGameObject(), stream );

   stream.close();
}

void AddComponentOnGUI( Editor& editor, GameObject& active )
{
   if (ImGui::BeginMenu( "Add component" ))
   {
      for (int i = 0; i < COMPONENT_COUNT; i++)
      {
         if (ComponentCreator::Instance().ComponentIDIsValid( i ))
         {
            if (ImGui::Selectable( ComponentCreator::Instance().GetName( i ).c_str() ))
            {
               Component* newComp = ComponentCreator::Instance().Create( i );
               
               // the scene must be reloaded in order to awake the component.
               // this is to deal with components being added that are dependent on one another.
               active.AddComponent( *newComp );
            }
         }
      }

      ImGui::EndMenu();
   }
}