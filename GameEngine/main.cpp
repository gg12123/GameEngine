#include <Windows.h>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

int CALLBACK WinMain( HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine,
                      int nCmdShow )
{
   if (!glfwInit())
   {
      return;
   }

   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
   glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
   glfwWindowHint( GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
   glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

   GLFWwindow* window = glfwCreateWindow( 800, 600, "Title", NULL, NULL );

   return 0;
}