#pragma once
#include "EditorWindow.h"
#include "GameObjectCreationContext.h"
#include "ParentSetter.h"

class GameObject;

class HierarchyWindow : public EditorWindow
{
public:
   HierarchyWindow();

   void Awake( Editor& editor ) override;
   void Update() override;

private:
   void DrawTreeView( GameObject& obj, int id );
   void ContextMenu();

   Editor* m_Editor;
   GameObject* m_Root;
   ParentSetter m_ParentSetter;
   GameObjectCreationContext m_GOCreation;
};
