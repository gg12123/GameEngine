#include "EditorUtils.h"
#include "GameObject.h"
#include "Transform.h"
#include "Utils.h"
#include "ImGUI/imgui.h"
#include "ComponentCreator.h"
#include "Editor.h"
#include "Path.h"

GameObjectCreationContext::GameObjectCreationContext()
{
   m_CreationFunctions[ 0 ].Init( "Light", CreateLightGameObject );
   m_CreationFunctions[ 1 ].Init( "Camera", CreateCameraGameObject );
   m_CreationFunctions[ 2 ].Init( "Cube", CreateCubeGameObject );
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
            newObj.EditAwakeComponents( editor );
         }
      }

      ImGui::EndMenu();
   }
}

void PrefabSpawnContext::OnGUI( Editor& editor, GameObject& parent )
{
   if (ImGui::BeginMenu( "Spawn prefab" ))
   {
      if (m_AvailablePrefabs.size() == 0)
      {
         GetFileNamesInDirectory( Path::Instance().GetPrefabPath( "" ), m_AvailablePrefabs );
      }

      bool selected = false;

      for (auto it = m_AvailablePrefabs.begin(); it != m_AvailablePrefabs.end(); it++)
      {
         if (ImGui::Selectable( it->c_str() ))
         {
            PrefabField prefabField;
            prefabField.SetValue( *it );
            prefabField.InstantiateEdit( editor, parent.GetTransform() );

            selected = true;
            break;
         }
      }

      if (selected)
      {
         m_AvailablePrefabs.clear();
      }

      ImGui::EndMenu();
   }
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
               
               active.AddComponent( *newComp );
               newComp->EditAwake( editor, active );
            }
         }
      }

      ImGui::EndMenu();
   }
}