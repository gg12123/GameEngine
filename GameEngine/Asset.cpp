#include "Asset.h"

Asset::~Asset()
{
}

Mesh& Asset::MeshValue()
{
   throw std::exception("Asset is not a mesh");
}

GLuint Asset::ShaderValue()
{
   throw std::exception( "Asset is not a shader" );
}

PrefabAsset& Asset::PrefabValue()
{
   throw std::exception( "Asset is not a prefab" );
}
