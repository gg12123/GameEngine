#include "Mesh.h"
#include "Path.h"

Mesh::Mesh()
{
   m_Vertices = nullptr;
}

Mesh::~Mesh()
{
   if (m_Vertices)
   {
      delete[] m_Vertices;
   }
}

Asset* Mesh::CreateInstance()
{
   return new Mesh();
}

Mesh& Mesh::MeshValue()
{
   return *this;
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

   m_NumVertices = 8;
   m_NumIndices = 36;

   m_Vertices = new Vertex[ m_NumVertices ];
   int index = 0;

   for (int i = 0; i < m_NumVertices; i++)
   {
      m_Vertices[ i ].Position = vmath::vec4( positions[ index ], positions[ index + 1 ], positions[ index + 2 ], 1.0f );

      index += 3;
   }

   glGenBuffers( 1, &m_VertexBuffer );
   glBindBuffer( GL_ARRAY_BUFFER, m_VertexBuffer );
   glBufferData( GL_ARRAY_BUFFER,
                 m_NumVertices * sizeof( Vertex ),
                 m_Vertices,
                 GL_STATIC_DRAW );

   glGenBuffers( 1, &m_IndicesBuffer );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndicesBuffer );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                 sizeof( indices ),
                 indices,
                 GL_STATIC_DRAW );

   m_Bounds.Calculate( m_Vertices, m_NumVertices );
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

MeshBounds& Mesh::GetBounds()
{
   return m_Bounds;
}
