#include "Editor.h"
#include "InspectorWindow.h"
#include "HierarchyWindow.h"
#include "World.h"
#include "Transform.h"
#include "GameObject.h"

Editor::Editor()
{
   m_World = nullptr;
   m_ActiveGameObject = nullptr;
}

void Editor::Awake( World& w )
{
   m_World = &w;

   m_ActiveGameObject = &(*m_World->GetRootTransform().ChildrenBegin())->GetGameObject();

   // maybe create the editor hierarchy here (camera, movement handles etc.)

   AddWindow( *(new HierarchyWindow()) );
   AddWindow( *(new InspectorWindow()) );
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
   // may need to notify objects of this change
}

GameObject* Editor::GetActiveGameObject()
{
   return m_ActiveGameObject;
}

World& Editor::GetWorld()
{
   return *m_World;
}