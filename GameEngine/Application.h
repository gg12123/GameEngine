#pragma once
#include "World.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

class Application
{
public:
   void Run();

private:

   bool InitWindow();
   void InitWorld();
   void InitImGUI();
   void RunLoop();
   void SetupCallbacks();

   GLFWwindow* m_Window;
   World m_World;
   WindowConfiguration m_WindowConfig;
};
