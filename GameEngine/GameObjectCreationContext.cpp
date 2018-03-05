#include "GameObjectCreationContext.h"
#include "GameObject.h"
#include "Transform.h"
#include "Utils.h"
#include "ImGUI/imgui.h"

GameObjectCreationContext::GameObjectCreationContext()
{
   m_CreationFunctions[ 0 ].Init( "Light", CreateLightGameObject );
   m_CreationFunctions[ 1 ].Init( "Camera", CreateCameraGameObject );
   m_CreationFunctions[ 2 ].Init( "Cube", CreateCubeGameObject );
}

GameObject* GameObjectCreationContext::OnGUI( GameObject& parent )
{
   GameObject* obj = nullptr;

   for (int i = 0; i < NUMBER_OFCREATABLE_GAME_OBJECTS; i++)
   {
      if (ImGui::Selectable( m_CreationFunctions[ i ].Name.c_str() ))
      {
         obj = &m_CreationFunctions[ i ].Function( vmath::vec3( 0.0f, 0.0f, 0.0f ), vmath::mat4().identity(), parent.GetTransform() );
      }
   }

   return obj;
}