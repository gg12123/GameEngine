#pragma once
#include "EditorWindow.h"

class GameObject;

class HierarchyWindow : public EditorWindow
{
public:
   HierarchyWindow();

   void Awake( Editor& editor ) override;
   void Update() override;

private:
   void DrawTreeView( GameObject& obj, int id );

   Editor* m_Editor;
   GameObject* m_Root;
};
