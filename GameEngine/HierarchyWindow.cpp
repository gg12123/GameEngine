#include "HierarchyWindow.h"
#include "Editor.h"
#include "World.h"
#include "Transform.h"

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

void HierarchyWindow::Update()
{
   
}