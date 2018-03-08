#include "GameObjectRenamer.h"
#include "ImGUI/imgui.h"
#include "GameObject.h"
#include "Utils.h"

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