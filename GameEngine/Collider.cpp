#include "Collider.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"
#include "Debug.h"
#include "Physics.h"

Collider::Collider()
{
   m_MeshBounds = nullptr;
}

void Collider::OnDestroy()
{
   if (!m_ToThisInPhysics.IsNull())
   {
      GetPhysics().UnRegisterCollider( m_ToThisInPhysics.Get() );
      m_ToThisInPhysics.Clear();
   }
}

void Collider::TryCacheMeshBounds()
{
   if (!m_MeshBounds)
   {
      MeshRenderer* meshRen = GetGameObject().GetComponent<MeshRenderer>();

      if (meshRen)
      {
         m_MeshBounds = &meshRen->GetMeshData().GetBounds();
      }
      else
      {
         Debug::Instance().LogError( "No mesh renderer on collider object" );
      }
   }
}

void Collider::ReCalculateBounds()
{
   if (m_MeshBounds)
   {
      vmath::vec3 scale = GetGameObject().GetTransform().GetScale();

      for (int i = 0; i < 3; i++)
      {
         m_Bounds[ i ][ 0 ] = scale[ i ] * m_MeshBounds->Min( i );
         m_Bounds[ i ][ 1 ] = scale[ i ] * m_MeshBounds->Max( i );
      }
   }
   else
   {
      Debug::Instance().LogError( "No mesh bounds to calculate collider bounds" );
   }
}

void Collider::Awake()
{
   TryCacheMeshBounds();
   ReCalculateBounds();
   m_ToThisInPhysics.Set( GetPhysics().RegisterCollider( *this ) );
   GetGameObject().RegisterForEvent( eOnDestroy, *m_OnDestroyEvent.Init( &Collider::OnDestroy, this ) );
}

void Collider::EditAwake( IEditor& editor )
{
   Awake();
}

float Collider::MinBound( unsigned int axis )
{
   return m_Bounds[ axis ][ 0 ];
}

float Collider::MaxBound( unsigned int axis )
{
   return m_Bounds[ axis ][ 1 ];
}