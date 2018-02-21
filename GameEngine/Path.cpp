#include "Path.h"

Path& Path::Instance()
{
   static Path instance;
   return instance;
}

std::string Path::GetMeshPath( std::string meshName )
{
   return "C:\\Users\\gg_11\\Documents\\Visual Studio 2015\\Projects\\GameEngine2\\Assets\\Meshes\\" + meshName;
}

std::string Path::GetShaderPath( std::string shaderName )
{
   return "C:\\Users\\gg_11\\Documents\\Visual Studio 2015\\Projects\\GameEngine2\\Assets\\Shaders\\" + shaderName;
}