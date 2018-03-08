#pragma once
#include "World.h"
#include "Editor.h"
#include "SceneLoader.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

class Application
{
public:
   void Run();

private:

   bool InitWindow();
   void InitWorld();
   void InitEditor();
   void InitImGUI();
   void RunLoop();
   void SetupCallbacks();

   GLFWwindow* m_Window;
   World m_World;
   WindowConfiguration m_WindowConfig;
   EditModeSceneLoader m_SceneLoader; // this is the edit mode application
   Editor m_Editor;
};
