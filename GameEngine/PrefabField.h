#pragma once
#include "SerializedFields.h"

class World;
class GameObject;

class PrefabField : public SerializedString
{
public:
   GameObject& Instantiate( const World& world ) const;
};