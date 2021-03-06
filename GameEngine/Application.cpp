#include "Application.h"
#include "Debug.h"
#include "Utils.h"
#include "Transform.h"
#include "GameObject.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "MainToolWindow.h"

Application* Application::m_ThisApp;

Application::Application()
{
   m_ThisApp = this;

   m_KeyMap[ GLFW_KEY_SPACE ] = eSpaceBar;
}

Input& Application::GetInput()
{
   return m_Input;
}

std::unordered_map<int, eInputKey>& Application::GetKeyMap()
{
   return m_KeyMap;
}

bool Application::InitWindow()
{
   bool ok = false;

   if (glfwInit())
   {
      glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
      glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
      glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
      glfwWindowHint( GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET );
      glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
      glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

      m_WindowConfig.Set( 1000.0f, 800.0f );

      m_Window = glfwCreateWindow( static_cast<int>(m_WindowConfig.GetWidth()),
                                   static_cast<int>(m_WindowConfig.GetHeight()),
                                   "Engine",
                                   NULL,
                                   NULL );

      if (m_Window)
      {
         ok = true;
         glfwMakeContextCurrent( m_Window );
         gl3wInit();
      }
   }

   return ok;
}

void Application::SetupCallbacks()
{
   glfwSetMouseButtonCallback( m_Window, MouseCallback );
   glfwSetScrollCallback( m_Window, ScrollCallback );
   glfwSetKeyCallback( m_Window, KeyCallback );
   glfwSetCharCallback( m_Window, CharCallback );
}

void Application::InitImGUI()
{
   ImGui::CreateContext();
   ImGui_ImplGlfwGL3_Init( m_Window, false );
   ImGui::StyleColorsDark();
}

void Application::Run()
{
   if (!InitWindow())
   {
      Debug::Instance().LogError( "Unable to open window" );
      return;
   }

   InitImGUI();
   SetupCallbacks();
   InitEngine();
   RunLoop();
}

void Application::InitEngine()
{
   m_World.Init( m_WindowConfig, m_SceneLoader, m_Input );
   m_Editor.Init( m_World );

   m_SceneLoader.Init( m_Editor );
   m_SceneLoader.LoadStartUpScene();

   m_Editor.AddWindow( *(new MainToolWindow()) );
   m_Editor.AddWindow( *(new HierarchyWindow()) );
   m_Editor.AddWindow( *(new InspectorWindow()) );
}

vmath::vec2 Application::GetCurrentMousePosition()
{
   double dx, dy;
   glfwGetCursorPos( m_Window, &dx, &dy );
   return vmath::vec2( static_cast<float>(dx), m_WindowConfig.GetHeight() - static_cast<float>(dy) );
}

void Application::RunLoop()
{
   bool running = true;

   Debug::Instance().Log( "starting main loop" );

   do
   {
      glfwPollEvents();
      ImGui_ImplGlfwGL3_NewFrame();

      m_Input.PreUpdate( GetCurrentMousePosition() );

      m_World.EditUpdate();
      m_Editor.Update();

      m_Input.FinalUpdate();

      ImGui::Render();
      ImGui_ImplGlfwGL3_RenderDrawData( ImGui::GetDrawData() );

      glfwSwapBuffers( m_Window );

      running &= (glfwGetKey( m_Window, GLFW_KEY_ESCAPE ) == GLFW_RELEASE);
      running &= (glfwWindowShouldClose( m_Window ) != GL_TRUE);

   } while (running);

   // Must clear here, otherwise an event gets invoked in the worlds destructor, which causes bad things to happen.
   m_World.ClearAll();
}

void Application::MouseCallback( GLFWwindow* window, int button, int action, int mods )
{
   ImGui_ImplGlfw_MouseButtonCallback( window, button, action, mods );

   if (!ImGui::GetIO().WantCaptureMouse)
   {
      if (GLFW_PRESS == action)
      {
         m_ThisApp->GetInput().OnMouseButtonDown( button );
      }
      else if (GLFW_RELEASE == action)
      {
         m_ThisApp->GetInput().OnMouseButtonUp( button );
      }
   }
}

void Application::ScrollCallback( GLFWwindow* w, double xoffset, double yoffset )
{
   ImGui_ImplGlfw_ScrollCallback( w, xoffset, yoffset );

   if (!ImGui::GetIO().WantCaptureMouse)
   {
      m_ThisApp->GetInput().OnScroll( (float)yoffset );
   }
}

void Application::KeyCallback( GLFWwindow* w, int key, int s, int action, int mods )
{
   ImGui_ImplGlfw_KeyCallback( w, key, s, action, mods );

   if (!ImGui::GetIO().WantCaptureKeyboard)
   {
      auto it = m_ThisApp->GetKeyMap().find( key );

      if (it != m_ThisApp->GetKeyMap().end())
      {
         if (GLFW_PRESS == action)
         {
            m_ThisApp->GetInput().OnKeyDown( it->second );
         }
         else if (GLFW_RELEASE == action)
         {
            m_ThisApp->GetInput().OnKeyUp( it->second );
         }
      }
   }
}

void Application::CharCallback( GLFWwindow* w, unsigned int c )
{
   ImGui_ImplGlfw_CharCallback( w, c );
}