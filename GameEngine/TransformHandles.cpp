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
#include "EditorEvents.h"

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

   RegisterForUpdate( eUpdateInEditMode );
}

bool TransformHandles::IsHandle( GameObject& obj )
{
   bool handle = false;

   if (&GetGameObject() == &obj)
   {
      handle = true;
   }
   else
   {
      for (auto it = m_Handles.begin(); it != m_Handles.end(); it++)
      {
         if (&(*it)->GetGameObject() == &obj)
         {
            handle = true;
            break;
         }
      }
   }

   return handle;
}

bool TransformHandles::TryGetHandle()
{
   Ray ray = GetActiveCamera().ScreenPointToRay( GetInput().MousePosition() );
   RayCastHit hit;
   bool handleFound = false;

   if (GetPhysics().RayCast( ray, hit ))
   {
      for (auto it = m_Handles.begin(); it != m_Handles.end(); it++)
      {
         if (&(*it)->AttachedCollider() == hit.HitCollider)
         {
            m_ActiveHandle = *it;
            handleFound = true;
            break;
         }
      }
   }
   return handleFound;
}

void TransformHandles::EditUpdate()
{
   GameObject *active = m_Editor->GetActiveGameObject();

   if (active && !IsHandle( *active ))
   {
      GetGameObject().GetTransform().SetPosition( active->GetTransform().GetPosition() );
      GetGameObject().GetTransform().SetRotation( active->GetTransform().GetRotation() );
      GetGameObject().GetTransform().SetScale( active->GetTransform().GetScale() );

      if (GetInput().MouseButtonDown( eLeftMouseButton ) && TryGetHandle())
      {
         m_ActiveHandle->StartPositionalMovement( active->GetTransform() );
      }

      if (GetInput().MouseButtonDown( eRightMouseButton ) && TryGetHandle())
      {
         m_ActiveHandle->StartScaleMovement( active->GetTransform() );
      }
   }

   if (GetInput().MouseButtonUp( eLeftMouseButton ) || GetInput().MouseButtonUp( eRightMouseButton ))
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