#pragma once
#include <string>
#include "GL/gl3w.h"
#include "Asset.h"

class Mesh : public Asset
{
public:

   static Asset* CreateInstance();

   void Load( std::string name ) override;

   int GetNumVertices();
   int GetNumIndices();
   GLuint GetVertexBuffer();
   GLuint GetIndicesBuffer();

private:

   int m_NumVertices;
   int m_NumIndices;
   GLuint m_VertexBuffer;
   GLuint m_IndicesBuffer;
};
