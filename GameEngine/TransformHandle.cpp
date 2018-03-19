#include "TransformHandle.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "ComponentIDs.h"
#include "Collider.h"
#include "Ray.h"
#include "Camera.h"

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

   RegisterForUpdate( eUpdateInEditMode );
}

void TransformHandle::EditUpdate()
{
   INVOKE_VOID_MEMBER_FUNC_PTR( this, m_ActiveMovementFunction );
}

int32_t TransformHandle::GetType()
{
   return COMPONENT_ID_TRANSFORMHANDLE;
}

float TransformHandle::CalculateInputMouseMovement()
{
   vec3 myPos = GetTransform().GetPosition();
   vec3 myDir = GetTransform().Forward();

   Ray r( myDir, myPos );

   vec2 inScreenSpace = normalize( GetActiveCamera().ToScreenSpaceDirection( r ) );
   vec2 mouseDelta = GetInput().MouseDelta();

   return (dot( mouseDelta, inScreenSpace ) * 0.01f);
}

void TransformHandle::ApplyPositionalMovement()
{
   const float movement = CalculateInputMouseMovement();

   Transform& parent = GetGameObject().GetTransform().GetParent();

   m_Target->SetPosition( parent.GetPosition() + movement * GetTransform().Forward() );
}

void TransformHandle::ApplyRotationalMovement()
{
   const float sensitivity = 1.0;
   mat4 rot = rotate( GetInput().MouseScroll() * sensitivity, GetTransform().Forward() );

   m_Target->SetRotation( rot * m_Target->GetRotation() );
}

void TransformHandle::ApplyScaleMovement()
{
   const float movement = CalculateInputMouseMovement();
   vec3 myDir = GetGameObject().GetTransform().Forward();

   int requiredDirection = 0;

   if (fabs( dot( m_Target->Forward(), myDir ) ) > 0.9f)
   {
      requiredDirection = 2;
   }
   else if (fabs( dot( m_Target->Up(), myDir ) ) > 0.9f)
   {
      requiredDirection = 1;
   }

   vec3 currScale = m_Target->GetScale();
   currScale[ requiredDirection ] += movement;

   m_Target->SetScale( currScale );
}

void TransformHandle::ApplyNoMovement()
{
}