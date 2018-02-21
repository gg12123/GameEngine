#include "Debug.h"

Debug& Debug::Instance()
{
   static Debug instance;
   return instance;
}

void Debug::LogError( std::string msg )
{
   // maybe use imgui here
}