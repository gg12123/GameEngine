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
   m_World->RegisterToEvent( eOnHierarchyBeingDestroyed, *m_OnHierarchyBeingDestroyedEvent.Init( &Editor::OnHierarchyBeingDestroyed, this ) );
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
   m_Events.Register( eventID, callback );
}

void Editor::InvokeEvent( EEditorEvent eventID )
{
   m_Events.Invoke( eventID );
}

void Editor::OnHierarchyBeingDestroyed( GameObject& root )
{
   if (m_ActiveGameObject && m_ActiveGameObject->IsPartOfHierarchy( root ))
   {
      SetActiveGameObject( nullptr );
   }
}