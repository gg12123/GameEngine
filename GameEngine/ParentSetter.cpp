#include "ParentSetter.h"
#include "GameObject.h"
#include "Transform.h"
#include "ImGUI/imgui.h"

ParentSetter::ParentSetter()
{
   m_PotentialChild = nullptr;
   m_PotentialParent = nullptr;
}

void ParentSetter::OnEndOfDrawing()
{
   if (!ImGui::IsMouseDragging())
   {
      if (m_PotentialChild && m_PotentialParent && (m_PotentialChild != m_PotentialParent))
      {
         m_PotentialChild->GetTransform().SetParent( m_PotentialParent->GetTransform() );
      }

      m_PotentialChild = nullptr;
      m_PotentialParent = nullptr;
   }
}

void ParentSetter::Draw( GameObject& gameObject )
{
   ImGui::SameLine();
   ImGui::SmallButton( "Parent" );

   if (ImGui::IsItemActive())
   {
      ImGuiIO& io = ImGui::GetIO();
      ImDrawList* drawList = ImGui::GetWindowDrawList();

      drawList->PushClipRectFullScreen();
      drawList->AddLine( io.MouseClickedPos[ 0 ], io.MousePos, ImGui::GetColorU32( ImGuiCol_Button ), 4.0f );
      drawList->PopClipRect();

      m_PotentialChild = &gameObject;
   }
   
   if (ImGui::IsItemHovered( ImGuiHoveredFlags_AllowWhenBlockedByActiveItem ))
   {
      m_PotentialParent = &gameObject;
   }
}