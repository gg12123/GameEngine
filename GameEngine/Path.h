#pragma once
#include <string>

class Path
{
public:

   static Path& Instance();

   std::string GetMeshPath( std::string meshName );
   std::string GetShaderPath( std::string shaderName );
};
