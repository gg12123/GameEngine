#include "GameObject.h"
#include "SerializedFields.h"
#include "Component.h"
#include "Editor.h"
#include "ImGUI/imgui.h"
#include "InspectorWindow.h"

static void CallOnGUI( std::unordered_map<std::string, SerializedField*>::iterator it )
{
   it->second->OnGUI( it->first );
}

static void CallInitGUI( std::unordered_map<std::string, SerializedField*>::iterator it )
{
   it->second->InitForGUI();
}

static void DrawText( std::vector<Component*>::iterator it )
{
   ImGui::Text( (*it)->GetName().c_str() );
}

static void DoNothing( std::vector<Component*>::iterator it )
{
}

InspectorWindow::InspectorWindow()
{
   m_Editor = nullptr;
}

void InspectorWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
  
   m_OnNewActiveGOEvent = new VoidEvent<InspectorWindow>( &InspectorWindow::OnActiveGameObjectChanged, *this );
  
   m_Editor->RegisterCallbackForEvent( eActiveGameObjectChanged, *m_OnNewActiveGOEvent );
}

void InspectorWindow::CallIntoSerializedFields( GameObject* active,
                                                InspectorGUIFunctionPtr1 function1,
                                                InspectorGUIFunctionPtr2 function2 )
{
   std::unordered_map<std::string, SerializedField*> fields;

   for (std::vector<Component*>::iterator it = active->ComponentsBegin();
         it != active->ComponentsEnd();
         it++)
   {
      fields.clear();
      (*it)->GetSerializedFields( fields );

      function1( it );

      for (std::unordered_map<std::string, SerializedField*>::iterator it2 = fields.begin();
            it2 != fields.end();
            it2++)
      {
         function2( it2 );
      }
   }
}

void InspectorWindow::Update()
{
   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != nullptr)
   {
      ImGui::Begin( "Inspector", 0, ImGuiWindowFlags_AlwaysAutoResize );
      ImGui::Text( active->GetName().c_str() );

      CallIntoSerializedFields( active, DrawText, CallOnGUI );

      ImGui::End();
   }
}

void InspectorWindow::OnActiveGameObjectChanged()
{
   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != nullptr)
   {
      CallIntoSerializedFields( active, DoNothing, CallInitGUI );
   }
}