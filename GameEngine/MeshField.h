#pragma once
#include "SerializedFields.h"

class World;

class MeshField : public SerializedString
{
public:
   std::string Instantiate( World& world );
};
