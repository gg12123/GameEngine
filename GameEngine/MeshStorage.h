#pragma once
#include <string>
#include <unordered_map>
#include "Mesh.h"

class MeshStorage
{
public:

   void LoadMeshIfNotAlreadyLoaded( std::string name );
   Mesh& GetMesh( std::string name );

private:

   std::unordered_map<std::string, Mesh*> m_Meshes;
};