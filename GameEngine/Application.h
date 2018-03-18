#pragma once
#include "World.h"
#include "Editor.h"
#include "SceneLoader.h"
#include "Input.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

class Application
{
public:
   Application();
   void Run();

   Input& GetInput();

private:

   static void MouseCallback( GLFWwindow*, int button, int action, int /*mods*/ );
   static void ScrollCallback( GLFWwindow*, double xoffset, double yoffset );
   static void KeyCallback( GLFWwindow*, int key, int, int action, int mods );
   static void CharCallback( GLFWwindow*, unsigned int c );

   bool InitWindow();
   void InitEngine();
   void InitImGUI();
   void RunLoop();
   void SetupCallbacks();

   vmath::vec2 GetCurrentMousePosition();

   GLFWwindow* m_Window;
   World m_World;
   WindowConfiguration m_WindowConfig;
   EditModeSceneLoader m_SceneLoader; // this is the edit mode application
   Editor m_Editor;
   Input m_Input;

   static Application* m_ThisApp;
};
