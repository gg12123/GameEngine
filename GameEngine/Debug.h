#pragma once
#include <string>
#include <iostream>
#include <fstream>

class Debug
{
public:
   ~Debug();

   static Debug& Instance();

   void LogError( std::string msg );
   void Log( std::string msg );

private:
   Debug();

   std::ofstream m_File;
};
