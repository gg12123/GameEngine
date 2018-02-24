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
   m_CreationFunctions[ COMPONENT_ID_CAMERA ] = CreateCamera;
   m_CreationFunctions[ COMPONENT_ID_LIGHT ] = CreateLight;
   m_CreationFunctions[ COMPONENT_ID_TRANSFORM ] = CreateTransform;
   m_CreationFunctions[ COMPONENT_ID_MESHRENDERER ] = CreateMeshRenderer;
}

Component* ComponentCreator::Create( int32_t id )
{
   if (id >= COMPONENT_COUNT)
   {
      throw std::exception( "Component ID out of range" );
   }

   return m_CreationFunctions[ id ]();
}

ComponentCreator& ComponentCreator::Instance()
{
   static ComponentCreator inst;
   return inst;
}