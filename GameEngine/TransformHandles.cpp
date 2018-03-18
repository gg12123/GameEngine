#include "TransformHandles.h"
#include "Debug.h"
#include "GameObject.h"
#include "ComponentIDs.h"
#include "Ray.h"
#include "Physics.h"
#include "Camera.h"
#include "Collider.h"
#include "IEditor.h"
#include "Transform.h"

TransformHandles::TransformHandles()
{
   m_ActiveHandle = nullptr;
}

void TransformHandles::EditAwake( IEditor& editor )
{
   m_Editor = &editor;
   GetGameObject().GetComponentsInChildren<TransformHandle>( m_Handles );

   if (m_Handles.size() != 3)
   {
      Debug::Instance().LogError( "wrong number of transform handles" );
   }
}

void TransformHandles::EditUpdate()
{
   if (GetInput().MouseButtonDown( eLeftMouseButton ))
   {
      Ray ray = GetActiveCamera().ScreenPointToRay( GetInput().MousePosition() );
      RayCastHit hit;

      if (GetPhysics().RayCast( ray, hit ))
      {
         for (auto it = m_Handles.begin(); it != m_Handles.end(); it++)
         {
            if (&(*it)->AttachedCollider() == hit.HitCollider)
            {
               m_ActiveHandle = *it;
               m_ActiveHandle->StartPositionalMovement( hit.HitCollider->GetGameObject().GetTransform() );
            }
         }
      }
   }

   if (GetInput().MouseButtonUp( eLeftMouseButton ))
   {
      if (m_ActiveHandle)
      {
         m_ActiveHandle->StopMovement();
         m_ActiveHandle = nullptr;
      }
   }
}

int32_t TransformHandles::GetType()
{
   return COMPONENT_ID_TRANSFORMHANDLES;
}

void TransformHandles::OnNewActiveGO()
{
   if (m_ActiveHandle)
   {
      Debug::Instance().LogError( "Active GO changed whilst trasform handle is active " );
      m_ActiveHandle->StopMovement();
   }
   m_ActiveHandle = nullptr;

   GameObject *active = m_Editor->GetActiveGameObject();

   if (active)
   {
      GetGameObject().GetTransform().SetPosition( active->GetTransform().GetPosition() );
      GetGameObject().GetTransform().SetRotation( active->GetTransform().GetRotation() );
   }
}