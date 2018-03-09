#pragma once
#include "EditorWindow.h"
#include "EditorUtils.h"

class MainToolWindow : public EditorWindow
{
public:
   MainToolWindow();
   void Awake( Editor& editor ) override;
   void Update() override;
private:
   Editor* m_Editor;
   EditorSceneManagement m_SceneManagement;
};
