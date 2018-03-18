#pragma once
#include "Component.h"
#include "MemberFunctionPtrs.h"

class Collider;

class TransformHandle : public Component
{
public:
   TransformHandle();

   int32_t GetType() override;
   void EditUpdate() override;

   void StartPositionalMovement( Transform& target );
   void StartRotationalMovement( Transform& target );
   void StartScaleMovement( Transform& target );
   void StopMovement();

   Collider& AttachedCollider();

protected:
   void EditAwake( IEditor& editor ) override;

private:
   void ApplyPositionalMovement();
   void ApplyRotationalMovement();
   void ApplyScaleMovement();
   void ApplyNoMovement();

   float CalculateInputMouseMovement();

   Transform* m_Target;
   Collider* m_Collider;

   DECLARE_VOID_MEMBER_FUNC_PTR( TransformHandle, m_ActiveMovementFunction );
};

