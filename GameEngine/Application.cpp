#include "Application.h"
#include "sb7.h"
#include "Debug.h"
#include "Utils.h"

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

      m_WindowConfig.Set( 800, 600 );

      m_Window = glfwCreateWindow( m_WindowConfig.GetWidth(),
                                   m_WindowConfig.GetHeight(),
                                   "Engine",
                                   NULL,
                                   NULL );

      if (m_Window)
         ok = true;
   }
}

void Application::SetupCallbacks()
{

}

void Application::Run()
{
   if (!InitWindow())
   {
      Debug::Instance().LogError( "Unable to open window" );
      return;
   }

   SetupCallbacks();
   InitWorld();
   RunLoop();
}

void Application::InitWorld()
{
   // DeSerialize here if there is a start up secene available

   GameObject& root = HierarchyForNewProject();

   m_World.Awake( root, m_WindowConfig );
}

void Application::RunLoop()
{
   bool running = true;

   do
   {
      m_World.EditUpdate();

      glfwSwapBuffers( m_Window );
      glfwPollEvents();

      running &= (glfwGetKey( m_Window, GLFW_KEY_ESCAPE ) == GLFW_RELEASE);
      running &= (glfwWindowShouldClose( m_Window ) != GL_TRUE);

   } while (running);
}