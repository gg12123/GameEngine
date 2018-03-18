#include "ColourOnRayHit.h"
#include "ComponentIDs.h"
#include "Camera.h"
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"

void ColourOnRayHit::GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields )
{
   fields[ "spawnable" ] = &m_SpawnPrefab;
}

ColourOnRayHit::ColourOnRayHit()
{
   m_Colours[ 0 ] = vmath::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
   m_Colours[ 1 ] = vmath::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
   m_Colours[ 2 ] = vmath::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
   m_NextColour = 0;
}

void ColourOnRayHit::EditUpdate()
{
   if (GetInput().MouseButtonDown(eLeftMouseButton))
   {
      Ray ray = GetActiveCamera().ScreenPointToRay( GetInput().MousePosition() );
      RayCastHit hit;

      if (GetPhysics().RayCast( ray, hit ))
      {
         if (m_MyCollider == hit.HitCollider)
         {
            m_MyMeshRen->SetColour( m_Colours[ m_NextColour ] );
            m_NextColour = (m_NextColour + 1) % NUM_COLOURS;
         }
      }

      GameObject& spawned = InstantiatePrefab( m_SpawnPrefab );
      spawned.GetTransform().SetPosition( ray.GetOrigin() + 10.0f * ray.GetDirection() );
   }
}

int32_t ColourOnRayHit::GetType()
{
   return COMPONENT_ID_COLURRAYHIT;
}

void ColourOnRayHit::EditAwake( IEditor& editor )
{
   m_MyCollider = GetGameObject().GetComponent<Collider>();
   m_MyMeshRen = GetGameObject().GetComponent<MeshRenderer>();
   RegisterForUpdate( eUpdateInEditMode );
}