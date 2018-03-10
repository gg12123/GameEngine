#include "GameObject.h"
#include "SerializedFields.h"
#include "Component.h"
#include "Editor.h"
#include "ImGUI/imgui.h"
#include "InspectorWindow.h"
#include "ISearializedFieldOwner.h"

static void CallOnGUI( std::unordered_map<std::string, SerializedField*>::iterator it, Component& comp )
{
   it->second->OnGUI( it->first, comp );
}

static void CallInitGUI( std::unordered_map<std::string, SerializedField*>::iterator it, Component& comp )
{
   it->second->InitForGUI( it->first, comp );
}

static void CallOnGUIClose( std::unordered_map<std::string, SerializedField*>::iterator it, Component& comp )
{
   it->second->OnGUIClose();
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
   m_ActiveGO = nullptr;
}

void InspectorWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
   m_Editor->RegisterCallbackForEvent( eActiveGameObjectChanged, *m_NewActiveGOEvent.Init( &InspectorWindow::OnNewActiveGO, this ) );
}

void InspectorWindow::CallIntoSerializedFields( GameObject* active,
                                                InspectorGUIFunctionPtr1 eachCompFunction,
                                                InspectorGUIFunctionPtr2 eachFieldFunction )
{
   std::unordered_map<std::string, SerializedField*> fields;

   for (std::vector<Component*>::iterator it = active->ComponentsBegin();
         it != active->ComponentsEnd();
         it++)
   {
      fields.clear();
      (*it)->GetSerializedFields( fields );

      eachCompFunction( it );

      for (std::unordered_map<std::string, SerializedField*>::iterator it2 = fields.begin();
            it2 != fields.end();
            it2++)
      {
         eachFieldFunction( it2, **it );
      }
   }
}

void InspectorWindow::Update()
{
   if (m_ActiveGO)
   {
      ImGui::Begin( "Inspector", 0, ImGuiWindowFlags_AlwaysAutoResize );
      ImGui::Text( m_ActiveGO->GetName().c_str() );

      CallIntoSerializedFields( m_ActiveGO, DrawText, CallOnGUI );

      ImGui::End();
   }
}

void InspectorWindow::OnNewActiveGO()
{
   if (m_ActiveGO)
   {
      CallIntoSerializedFields( m_ActiveGO, DoNothing, CallOnGUIClose );
   }

   m_ActiveGO = m_Editor->GetActiveGameObject();

   if (m_ActiveGO)
   {
      CallIntoSerializedFields( m_ActiveGO, DoNothing, CallInitGUI );
   }
}