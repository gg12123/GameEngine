#include "Mesh.h"
#include "Path.h"

Asset* Mesh::CreateInstance()
{
   return new Mesh();
}

void Mesh::Load( std::string name )
{
   std::string path = Path::Instance().GetMeshPath( name );

   static const GLfloat positions[] =
   {
      -0.25f, -0.25f, -0.25f,
      -0.25f,  0.25f, -0.25f,
       0.25f, -0.25f, -0.25f,
       0.25f,  0.25f, -0.25f,
       0.25f, -0.25f,  0.25f,
       0.25f,  0.25f,  0.25f,
      -0.25f, -0.25f,  0.25f,
      -0.25f,  0.25f,  0.25f,
   };

   static const GLushort indices[] =
   {
      0, 1, 2,
      2, 1, 3,
      2, 3, 4,
      4, 3, 5,
      4, 5, 6,
      6, 5, 7,
      6, 7, 0,
      0, 7, 1,
      6, 0, 2,
      2, 4, 6,
      7, 5, 3,
      7, 3, 1
   };

   glGenBuffers( 1, &m_VertexBuffer );
   glBindBuffer( GL_ARRAY_BUFFER, m_VertexBuffer );
   glBufferData( GL_ARRAY_BUFFER,
                 sizeof( positions ),
                 positions,
                 GL_STATIC_DRAW );

   glGenBuffers( 1, &m_IndicesBuffer );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndicesBuffer );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                 sizeof( indices ),
                 indices,
                 GL_STATIC_DRAW );
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
