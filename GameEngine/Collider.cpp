#include "Collider.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"

Collider::Collider()
{
   m_MeshBounds = nullptr;
}

void Collider::Awake()
{
   MeshRenderer* meshRen = GetGameObject().GetComponent<MeshRenderer>();

   if (!meshRen)
   {
      throw std::exception( "No mesh renderer on collider object" );
   }

   m_MeshBounds = &meshRen->GetMeshData().GetBounds();

   // register
}

void Collider::EditAwake( IEditor& editor )
{
   Awake();
}

float Collider::CalculateMinBound( unsigned int axis )
{
   return m_Scale[ axis ] * m_MeshBounds->Min( axis );
}

float Collider::CalculateGetMaxBound( unsigned int axis )
{
   return m_Scale[ axis ] * m_MeshBounds->Max( axis );
}

void Collider::CacheScale()
{
   m_Scale = GetGameObject().GetTransform().GetScale();
}