#pragma once
#include <string>

class Debug
{
public:
   static Debug& Instance();

   void LogError( std::string msg );
};
