#pragma once
#include "Component.h"
#include "ComponentIDs.h"

typedef Component*(*ComponentCreationFunctionPtr)();

struct ComponentInfo
{
   ComponentCreationFunctionPtr CreationFunc;
   std::string Name;

   void Init( std::string name, ComponentCreationFunctionPtr func )
   {
      Name = name;
      CreationFunc = func;
   }
};

class ComponentCreator
{
public:
   static ComponentCreator& Instance();
   Component* Create( int32_t id );
   std::string GetName( int32_t id );

private:
   ComponentCreator();
   ComponentInfo m_ComponentInfo[ COMPONENT_COUNT ];
};
