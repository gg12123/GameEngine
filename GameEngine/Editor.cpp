#include <algorithm>
#include "Editor.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"
#include "World.h"
#include "Transform.h"
#include "GameObject.h"
#include "Debug.h"

Editor::Editor()
{
   m_World = nullptr;
   m_ActiveGameObject = nullptr;
}

Editor::~Editor()
{
   for (auto it = m_CurrentWindows.begin(); it != m_CurrentWindows.end(); it++)
   {
      delete *it;
   }
}

void Editor::Init( World& w )
{
   m_World = &w;
}

void Editor::AddWindow( EditorWindow& wnd )
{
   wnd.Awake( *this );
   m_CurrentWindows.push_back( &wnd );
}

void Editor::Update()
{
   for (auto it = m_CurrentWindows.begin(); it != m_CurrentWindows.end(); it++)
   {
      (*it)->Update();
   }
}

void Editor::SetActiveGameObject( GameObject* active )
{
   m_ActiveGameObject = active;
   InvokeEvent( eActiveGameObjectChanged );
}

GameObject* Editor::GetActiveGameObject()
{
   return m_ActiveGameObject;
}

World& Editor::GetWorld()
{
   return *m_World;
}

void Editor::RegisterCallbackForEvent( EEditorEvent eventID, EventHandler& callback )
{
   auto it = m_Events.find( eventID );
   std::vector<EventHandler*>* v;

   if (it != m_Events.end())
   {
      v = it->second;
   }
   else
   {
      v = new std::vector<EventHandler*>();
      m_Events[ eventID ] = v;
   }

   v->push_back( &callback );
}

void Editor::UnregisterCallback( EEditorEvent eventID, EventHandler& callback )
{
   auto it = m_Events.find( eventID );

   if (it != m_Events.end())
   {
      std::vector<EventHandler*>* v = it->second;

      auto it2 = std::find( v->begin(), v->end(), &callback );

      if (it2 != v->end())
      {
         v->erase( it2 );
      }
      else
      {
         Debug::Instance().LogError( "Unable to find callback to un reg event" );
      }
   }
   else
   {
      Debug::Instance().LogError( "Unable to event ID to un reg event" );
   }
}

void Editor::InvokeEvent( EEditorEvent eventID )
{
   auto it = m_Events.find( eventID );

   if (it != m_Events.end())
   {
      std::vector<EventHandler*>* v = it->second;

      for (auto it2 = v->begin(); it2 != v->end(); it2++)
      {
         (*it2)->Invoke();
      }
   }
}