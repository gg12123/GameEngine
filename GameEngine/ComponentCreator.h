#pragma once
#include "Component.h"
#include "ComponentIDs.h"

typedef Component*(*ComponentCreationFunctionPtr)();

class ComponentCreator
{
public:
   static ComponentCreator& Instance();
   Component* Create( int32_t id );

private:
   ComponentCreator();
   ComponentCreationFunctionPtr m_CreationFunctions[ COMPONENT_COUNT ];
};
