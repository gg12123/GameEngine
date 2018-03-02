#include "InspectorWindow.h"
#include "GameObject.h"
#include "SerializedFields.h"
#include "Component.h"
#include "Editor.h"
#include "ImGUI/imgui.h"

static void CallOnGUI( std::unordered_map<std::string, SerializedField*>::iterator it )
{
   it->second->OnGUI( it->first );
}

static void CallInitGUI( std::unordered_map<std::string, SerializedField*>::iterator it )
{
   it->second->InitForGUI();
}

InspectorWindow::InspectorWindow()
{
   m_Editor = nullptr;
}

void InspectorWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
}

void InspectorWindow::CallIntoSerializedFields( GameObject* active, InspectorGUIFunctionPtr function )
{
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
         function( it2 );
      }
   }
}

void InspectorWindow::Update()
{
   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != nullptr)
   {
      ImGui::Begin( "Inspector", 0, ImGuiWindowFlags_AlwaysAutoResize );

      CallIntoSerializedFields( active, CallOnGUI );

      ImGui::End();
   }
}

void InspectorWindow::OnActiveGameObjectChanged()
{
   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != nullptr)
   {
      CallIntoSerializedFields( active, CallInitGUI );
   }
}