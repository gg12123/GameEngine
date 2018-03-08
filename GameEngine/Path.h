#pragma once
#include <string>

class Path
{
public:

   static Path& Instance();

   std::string GetMeshPath( std::string meshName );
   std::string GetShaderPath( std::string shaderName );
   std::string GetDebugPath( std::string fileName );
   std::string GetScenePath( std::string sceneName );
   std::string GetPrefabPath( std::string profabName );
};
