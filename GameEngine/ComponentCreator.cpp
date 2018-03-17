#include "ComponentCreator.h"
#include "DiffuseMeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "ColourOnRayHit.h"

// ###################
static Component* CreateDiffuseMeshRenderer()
{
   return new DiffuseMeshRenderer();
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

// ###################
static Component* CreateBoxCollider()
{
   return new BoxCollider();
}

// ###################
static Component* CreateColourOnRay()
{
   return new ColourOnRayHit();
}

ComponentCreator::ComponentCreator()
{
   m_ComponentInfo[ COMPONENT_ID_CAMERA ].Init( "Camera", CreateCamera );
   m_ComponentInfo[ COMPONENT_ID_LIGHT ].Init( "Light", CreateLight );
   m_ComponentInfo[ COMPONENT_ID_TRANSFORM ].Init( "Transform", CreateTransform );
   m_ComponentInfo[ COMPONENT_ID_DIFFUSEMESHRENDERER ].Init( "Diffuse mesh renderer", CreateDiffuseMeshRenderer );
   m_ComponentInfo[ COMPONENT_ID_BOXCOLLIDER ].Init( "Box collider", CreateBoxCollider );
   m_ComponentInfo[ COMPONENT_ID_COLURRAYHIT ].Init( "Change colour on click", CreateColourOnRay );
}

bool ComponentCreator::ComponentIDIsValid( int32_t id )
{
   return (id < COMPONENT_COUNT) && (m_ComponentInfo[ id ].CreationFunc); // so an obsolete id must be assigned a null function
}

Component* ComponentCreator::Create( int32_t id )
{
   if (id >= COMPONENT_COUNT)
   {
      throw std::exception( "Component ID out of range" );
   }

   if (!m_ComponentInfo[ id ].CreationFunc)
   {
      throw std::exception( "Component is obsolete" );
   }

   return m_ComponentInfo[ id ].CreationFunc();
}

std::string ComponentCreator::GetName( int32_t id )
{
   if (id >= COMPONENT_COUNT)
   {
      throw std::exception( "Component ID out of range" );
   }

   if (!m_ComponentInfo[ id ].CreationFunc)
   {
      throw std::exception( "Component is obsolete" );
   }

   return m_ComponentInfo[ id ].Name;
}

ComponentCreator& ComponentCreator::Instance()
{
   static ComponentCreator inst;
   return inst;
}