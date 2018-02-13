#pragma once
#include <string>
#include <map>
#include "Mesh.h"

class MeshStorage
{
public:

   void LoadMeshIfNotAlreadyLoaded( std::string name );
   Mesh& GetMesh( std::string name );

private:

   std::map<std::string, Mesh*> m_Meshes;
};