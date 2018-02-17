#include "Asset.h"

Mesh* Asset::MeshValue()
{
   throw std::exception("Asset is not a mesh");
}

GLuint Asset::ShaderValue()
{
   throw std::exception( "Asset is not a shader" );
}
