#include "TransformHandle.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "ComponentIDs.h"
#include "Collider.h"

TransformHandle::TransformHandle()
{
   m_Target = nullptr;
   m_Collider = nullptr;
}

Collider& TransformHandle::AttachedCollider()
{
   return *m_Collider;
}

void TransformHandle::StartPositionalMovement( Transform& target )
{
   m_Target = &target;
   m_ActiveMovementFunction = &TransformHandle::ApplyPositionalMovement;
}

void TransformHandle::StartRotationalMovement( Transform& target )
{
   m_Target = &target;
   m_ActiveMovementFunction = &TransformHandle::ApplyRotationalMovement;
}

void TransformHandle::StartScaleMovement( Transform& target )
{
   m_Target = &target;
   m_ActiveMovementFunction = &TransformHandle::ApplyScaleMovement;
}

void TransformHandle::StopMovement()
{
   m_Target = nullptr;
   m_ActiveMovementFunction = &TransformHandle::ApplyNoMovement;
}

void TransformHandle::EditAwake( IEditor& editor )
{
   m_ActiveMovementFunction = &TransformHandle::ApplyNoMovement;
   m_Collider = GetGameObject().GetComponent<Collider>();

   if (!m_Collider)
      Debug::Instance().LogError( "No collider attached to transform handle" );
}

void TransformHandle::EditUpdate()
{
   INVOKE_VOID_MEMBER_FUNC_PTR( this, m_ActiveMovementFunction );
}

int32_t TransformHandle::GetType()
{
   return COMPONENT_ID_TRANSFORMHANDLE;
}

void TransformHandle::ApplyPositionalMovement()
{
   // move target in the direction of my forward.
}

void TransformHandle::ApplyRotationalMovement()
{
   // rotate target about my forward.
}

void TransformHandle::ApplyScaleMovement()
{
   // scale target in the direction of my forward.
}

void TransformHandle::ApplyNoMovement()
{

}