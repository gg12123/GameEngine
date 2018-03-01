#include "Application.h"
#include "sb7.h"
#include "Debug.h"
#include "Utils.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw_gl3.h"

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

      m_WindowConfig.Set( 1000, 800 );

      m_Window = glfwCreateWindow( m_WindowConfig.GetWidth(),
                                   m_WindowConfig.GetHeight(),
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

}

void Application::InitImGUI()
{
   ImGui::CreateContext();
   ImGui_ImplGlfwGL3_Init( m_Window, true );
   ImGui::StyleColorsDark();
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
   InitImGUI();
   RunLoop();
}

void Application::InitWorld()
{
   // DeSerialize here if there is a start up secene available

   std::vector<GameObject*> objects;

   GameObject& root = HierarchyForNewProject( objects );

   m_World.Awake( root, objects, m_WindowConfig );
}

static void TestOutImGui()
{
   static float f;

   ImGui::InputFloat( "test float", &f );
}

void Application::RunLoop()
{
   bool running = true;

   do
   {
      glfwPollEvents();
      ImGui_ImplGlfwGL3_NewFrame();

      m_World.EditUpdate();
      TestOutImGui();

      ImGui::Render();
      ImGui_ImplGlfwGL3_RenderDrawData( ImGui::GetDrawData() );

      glfwSwapBuffers( m_Window );

      running &= (glfwGetKey( m_Window, GLFW_KEY_ESCAPE ) == GLFW_RELEASE);
      running &= (glfwWindowShouldClose( m_Window ) != GL_TRUE);

   } while (running);
}