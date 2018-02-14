#include "MeshStorage.h"
#include "Debug.h"

void MeshStorage::LoadMeshIfNotAlreadyLoaded( std::string name )
{
   if (m_Meshes.count( name ) == 0)
   {
      Mesh *m = new Mesh();

      m_Meshes[ name ] = m;
      m->Load( name );
   }
}

Mesh& MeshStorage::GetMesh( std::string name )
{
   if (m_Meshes.count( name ) == 0)
   {
      Debug::Instance().LogError( "Mesh not available on request" );
      LoadMeshIfNotAlreadyLoaded( name );
   }

   return *(m_Meshes[ name ]);
}