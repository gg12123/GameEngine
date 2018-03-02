#include "InspectorWindow.h"
#include "GameObject.h"
#include "SerializedFields.h"
#include "Component.h"
#include "Editor.h"
#include "ImGUI/imgui.h"

InspectorWindow::InspectorWindow()
{
   m_Editor = nullptr;
}

void InspectorWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
}

void InspectorWindow::Update()
{
   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != nullptr)
   {
      ImGui::Begin( "Inspector", 0, ImGuiWindowFlags_AlwaysAutoResize );

      std::unordered_map<std::string, SerializedField*> fields;

      for (std::vector<Component*>::iterator it = active->ComponentsBegin();
            it != active->ComponentsEnd();
            it++)
      {
         fields.clear();
         (*it)->GetSerializedFields( fields );

         ImGui::Text( (*it)->GetName().c_str() );

         for (std::unordered_map<std::string, SerializedField*>::iterator it2 = fields.begin();
               it2 != fields.end();
               it2++)
         {
            it2->second->OnGUI( it2->first );
         }
      }

      ImGui::End();
   }
}