#pragma once
#include "Component.h"

class ComponentCreator
{
public:
   static ComponentCreator& Instance();
   Component* Create( int32_t id );
};
