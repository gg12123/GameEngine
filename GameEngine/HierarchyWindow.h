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

   Editor* m_Editor;
   GameObject* m_Root;
};
