#include "InspectorWindow.h"
#include "GameObject.h"
#include "SerializedFields.h"
#include "Component.h"
#include "ImGUI/imgui.h"

InspectorWindow::InspectorWindow()
{
   m_ActiveGameObject = nullptr;
}

void InspectorWindow::SetActiveGameObject( GameObject * obj )
{
   m_ActiveGameObject = obj;
}

void InspectorWindow::OnGUI()
{
   if (m_ActiveGameObject != nullptr)
   {
      ImGui::Begin( "Inspector" );

      std::unordered_map<std::string, SerializedField*> fields;

      for (std::vector<Component*>::iterator it = m_ActiveGameObject->ComponentsBegin();
            it != m_ActiveGameObject->ComponentsEnd();
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