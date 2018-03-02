#pragma once
#include "EditorWindow.h"

class GameObject;

class InspectorWindow : public EditorWindow
{
public:
   InspectorWindow();

   void Awake( Editor& editor ) override;
   void Update() override;

private:
   Editor* m_Editor;
};
