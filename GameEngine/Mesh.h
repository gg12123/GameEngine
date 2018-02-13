#pragma once
#include <string>
#include "GL/gl3w.h"

class Mesh
{
public:

   void Load( std::string path );

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
