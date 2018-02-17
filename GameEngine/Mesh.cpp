#include "Mesh.h"
#include "Path.h"

Asset* Mesh::CreateInstance()
{
   return new Mesh();
}

void Mesh::Load( std::string name )
{
   std::string path = Path::Instance().GetMeshPath( name );
}

int Mesh::GetNumVertices()
{
   return m_NumVertices;
}

int Mesh::GetNumIndices()
{
   return m_NumIndices;
}

GLuint Mesh::GetVertexBuffer()
{
   return m_VertexBuffer;
}

GLuint Mesh::GetIndicesBuffer()
{
   return m_IndicesBuffer;
}
