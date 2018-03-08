#include "GameObjectCreationContext.h"
#include "GameObject.h"
#include "Transform.h"
#include "Utils.h"
#include "ImGUI/imgui.h"
#include "ComponentCreator.h"

GameObjectCreationContext::GameObjectCreationContext()
{
   m_CreationFunctions[ 0 ].Init( "Light", CreateLightGameObject );
   m_CreationFunctions[ 1 ].Init( "Camera", CreateCameraGameObject );
   m_CreationFunctions[ 2 ].Init( "Cube", CreateCubeGameObject );
}

void GameObjectCreationContext::OnGUI( World& world, GameObject& parent )
{
   if (ImGui::BeginMenu( "New game object" ))
   {
      for (int i = 0; i < NUMBER_OF_CREATABLE_GAME_OBJECTS; i++)
      {
         if (ImGui::Selectable( m_CreationFunctions[ i ].Name.c_str() ))
         {
            GameObject& newObj = m_CreationFunctions[ i ].Function( vmath::vec3( 0.0f, 0.0f, 0.0f ), vmath::mat4().identity(), parent.GetTransform() );
            newObj.AwakeComponents( world );
         }
      }

      ImGui::EndMenu();
   }
}

void AddComponentOnGUI( World& world, GameObject& active )
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
               newComp->EditAwake( world, active );
            }
         }
      }

      ImGui::EndMenu();
   }
}