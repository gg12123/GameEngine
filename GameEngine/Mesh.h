#pragma once
#include <string>
#include "GL/gl3w.h"
#include "Asset.h"
#include "Vertex.h"
#include "MyVmath.h"

class MeshBounds
{
public:
   void Calculate( const Vertex *vertices, unsigned int count );

   float Min( unsigned int axis );
   float Max( unsigned int axis );

private:
   // 0, 1, 2 in m_Bounds for x, y, z.
   // 0, 1 in vec2 for min and max
   vmath::vec2 m_Bounds[ 3 ]; 
};

class Mesh : public Asset
{
public:

   ~Mesh();
   Mesh();

   static Asset* CreateInstance();

   void Load( std::string name ) override;
   Mesh& MeshValue() override;

   int GetNumVertices();
   int GetNumIndices();
   GLuint GetVertexBuffer();
   GLuint GetIndicesBuffer();
   MeshBounds& GetBounds();

private:

   int m_NumVertices;
   int m_NumIndices;
   GLuint m_VertexBuffer;
   GLuint m_IndicesBuffer;

   Vertex* m_Vertices;

   MeshBounds m_Bounds;
};
