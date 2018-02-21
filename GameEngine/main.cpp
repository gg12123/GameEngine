#include <Windows.h>
#include "Application.h"


int CALLBACK WinMain( HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine,
                      int nCmdShow )
{
   Application app;
   app.Run();
   return 0;
}