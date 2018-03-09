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
   m_PrevActiveGO = nullptr;
}

void InspectorWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
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
   CheckForActiveGOChanged();

   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != nullptr)
   {
      ImGui::Begin( "Inspector", 0, ImGuiWindowFlags_AlwaysAutoResize );
      ImGui::Text( active->GetName().c_str() );

      CallIntoSerializedFields( active, DrawText, CallOnGUI );

      ImGui::End();
   }
}

void InspectorWindow::CheckForActiveGOChanged()
{
   GameObject* active = m_Editor->GetActiveGameObject();

   if (active != m_PrevActiveGO)
   {
      if (m_PrevActiveGO)
      {
         CallIntoSerializedFields( m_PrevActiveGO, DoNothing, CallOnGUIClose );
      }

      if (active)
      {
         CallIntoSerializedFields( active, DoNothing, CallInitGUI );
      }

      m_PrevActiveGO = active;
   }
}