#pragma once
#include <string>
#include "GL/gl3w.h"

class Mesh;

class Asset
{
public:
   virtual void Load( std::string name ) = 0;
   virtual Mesh* MeshValue();
   virtual GLuint ShaderValue();
};
