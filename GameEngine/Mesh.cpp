#include "Mesh.h"
#include "Path.h"

void MeshBounds::Calculate( const Vertex *vertices, const unsigned int count )
{
   for (int axis = 0; axis < 3; axis++)
   {
      m_Bounds[ axis ][ 0 ] = vertices[ 0 ].Position[ axis ];
      m_Bounds[ axis ][ 1 ] = vertices[ 0 ].Position[ axis ];
   }

   for (int i = 1; i < count; i++)
   {
      for (int axis = 0; axis < 3; axis++)
      {
         // min
         if (vertices[ i ].Position[ axis ] < m_Bounds[ axis ][ 0 ])
         {
            m_Bounds[ axis ][ 0 ] = vertices[ i ].Position[ axis ];
         }

         // max
         if (vertices[ i ].Position[ axis ] > m_Bounds[ axis ][ 1 ])
         {
            m_Bounds[ axis ][ 1 ] = vertices[ i ].Position[ axis ];
         }
      }
   }
}

float MeshBounds::Min( const unsigned int axis ) const
{
   return m_Bounds[ axis ][ 0 ];
}

float MeshBounds::Max( const unsigned int axis ) const
{
   return m_Bounds[ axis ][ 1 ];
}


static void CalculateUnitCube(Vertex* vertices, GLushort* indicies)
{
   int index = 0;

   // x y plane
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
   index++;
   
   
   // x y plane
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, -1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, -1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, -1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, -1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, -1.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 0.0f, -1.0f, 0.0f );
   index++;
   
   // x z plane
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
   index++;
   
   // x z plane
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 0.0f, -1.0f, 0.0f, 0.0f );
   index++;
   
   // y z plane
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( 1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( 1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   
   // y z plane
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( -1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( -1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( -1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, 1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( -1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, 1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( -1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   vertices[ index ].Position = vmath::vec4( -1.0f, -1.0f, -1.0f, 1.0f );
   vertices[ index ].Normal = vmath::vec4( -1.0f, 0.0f, 0.0f, 0.0f );
   index++;
   
   vmath::mat4 scaleMat = vmath::scale( 0.5f, 0.5f, 0.5f );
   
   // scale to unit size
   for (unsigned int i = 0; i < 36; i++)
   {
      vertices[ i ].Position = scaleMat * vertices[ i ].Position;
      indicies[ i ] = i;
   }
}


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
   // will load from path in the final version but not for now.
   std::string path = Path::Instance().GetMeshPath( name );

   m_NumVertices = 36;
   m_NumIndices = 36;

   m_Vertices = new Vertex[ m_NumVertices ];
   GLushort* indices = new GLushort[ m_NumIndices ];

   CalculateUnitCube( m_Vertices, indices );

   glGenBuffers( 1, &m_VertexBuffer );
   glBindBuffer( GL_ARRAY_BUFFER, m_VertexBuffer );
   glBufferData( GL_ARRAY_BUFFER,
                 m_NumVertices * sizeof( Vertex ),
                 m_Vertices,
                 GL_STATIC_DRAW );

   glGenBuffers( 1, &m_IndicesBuffer );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndicesBuffer );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                 m_NumIndices * sizeof( GLushort ),
                 indices,
                 GL_STATIC_DRAW );

   m_Bounds.Calculate( m_Vertices, m_NumVertices );

   delete[] indices;
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
