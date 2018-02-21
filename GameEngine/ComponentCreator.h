#pragma once
#include "Component.h"

#define COMPONENT_ID_TRANSFORM    0
#define COMPONENT_ID_MESHRENDERER 1
#define COMPONENT_ID_CAMERA       2
#define COMPONENT_ID_LIGHT        3

class ComponentCreator
{
public:
   static ComponentCreator& Instance();
   Component* Create( int32_t id );
};
