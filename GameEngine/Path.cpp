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

std::string Path::GetDebugPath( std::string fileName )
{
   return "C:\\Users\\gg_11\\Documents\\Visual Studio 2015\\Projects\\GameEngine2\\DebugOutput\\" + fileName;
}

std::string Path::GetScenePath( std::string sceneName )
{
   return "C:\\Users\\gg_11\\Documents\\Visual Studio 2015\\Projects\\GameEngine2\\Assets\\Scenes\\" + sceneName;
}

std::string Path::GetPrefabPath( std::string prefabName )
{
   return "C:\\Users\\gg_11\\Documents\\Visual Studio 2015\\Projects\\GameEngine2\\Assets\\Prefabs\\" + prefabName;
}