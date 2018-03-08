#include "HierarchyWindow.h"
#include "Editor.h"
#include "World.h"
#include "Transform.h"
#include "ImGUI/imgui.h"
#include "GameObject.h"

HierarchyWindow::HierarchyWindow()
{
   m_Root = nullptr;
   m_Editor = nullptr;
}

void HierarchyWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
   m_Root = &editor.GetWorld().GetRootTransform().GetGameObject();
}

void HierarchyWindow::DrawTreeView( GameObject& obj, int id )
{
   if (ImGui::TreeNode( (void*)(intptr_t)id, obj.GetName().c_str() ))
   {
      ImGui::SameLine();

      if (ImGui::SmallButton( (m_Editor->GetActiveGameObject() == &obj) ? "Active" : "Set active" ))
      {
         m_Editor->SetActiveGameObject( &obj );
      }

      m_ParentSetter.Draw( obj );

      Transform& t = obj.GetTransform();
      int i = 0;

      for (auto it = t.ChildrenBegin(); it != t.ChildrenEnd(); it++)
      {
         DrawTreeView( (*it)->GetGameObject(), i );
         i++;
      }

      ImGui::TreePop();
   }
}

void HierarchyWindow::ContextMenu()
{
   int selected = -1;

   if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked( 1 ))
   {
      ImGui::OpenPopup( "HierarchyContext" );
   }

   if (ImGui::BeginPopup( "HierarchyContext" ))
   {
      GameObject* active = m_Editor->GetActiveGameObject();

      if (active)
      {
         m_GOCreation.OnGUI( *m_Editor, *active );
         AddComponentOnGUI( *m_Editor, *active );
         m_GORenamer.OnGUI( *active );
      }

      ImGui::EndPopup();
   }
}

void HierarchyWindow::Update()
{
   ImGui::Begin( "Hierarchy", 0, ImGuiWindowFlags_AlwaysAutoResize );

   DrawTreeView( *m_Root, 0 );
   m_ParentSetter.OnEndOfDrawing();
   ContextMenu();

   ImGui::End();
}