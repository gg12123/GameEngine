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
      if (ImGui::SmallButton( "Set active" ))
      {
         m_Editor->SetActiveGameObject( &obj );
      }

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

void HierarchyWindow::Update()
{
   ImGui::Begin( "Hierarchy", 0, ImGuiWindowFlags_AlwaysAutoResize );
   DrawTreeView( *m_Root, 0 );
   ImGui::End();
}