#pragma once
#include "SerializedFields.h"

class World;

class MeshField : public TypeInString
{
public:
   std::string Instantiate( World& world );
};
