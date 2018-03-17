#pragma once
#include <string>
#include "GL/gl3w.h"

class Mesh;
class PrefabAsset;
class GameObject;

class Asset
{
public:
   virtual ~Asset();

   virtual void Load( std::string name ) = 0;
   virtual Mesh& MeshValue();
   virtual GLuint ShaderValue();
   virtual PrefabAsset& PrefabValue();
};
