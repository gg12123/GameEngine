#include "ComponentCreator.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"

// ###################
static Component* CreateMeshRenderer()
{
   return new MeshRenderer();
}

// ###################
static Component* CreateCamera()
{
   return new Camera();
}

// ###################
static Component* CreateTransform()
{
   return new Transform();
}

// ###################
static Component* CreateLight()
{
   return new Light();
}

ComponentCreator::ComponentCreator()
{
   m_ComponentInfo[ COMPONENT_ID_CAMERA ].Init( "Camera", CreateCamera );
   m_ComponentInfo[ COMPONENT_ID_LIGHT ].Init( "Light", CreateLight );
   m_ComponentInfo[ COMPONENT_ID_TRANSFORM ].Init( "Transform", CreateTransform );
   m_ComponentInfo[ COMPONENT_ID_MESHRENDERER ].Init( "Mesh renderer", CreateMeshRenderer );
}

Component* ComponentCreator::Create( int32_t id )
{
   if (id >= COMPONENT_COUNT)
   {
      throw std::exception( "Component ID out of range" );
   }

   return m_ComponentInfo[ id ].CreationFunc();
}

std::string ComponentCreator::GetName( int32_t id )
{
   if (id >= COMPONENT_COUNT)
   {
      throw std::exception( "Component ID out of range" );
   }

   return m_ComponentInfo[ id ].Name;
}

ComponentCreator& ComponentCreator::Instance()
{
   static ComponentCreator inst;
   return inst;
}