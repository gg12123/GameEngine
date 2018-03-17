#pragma once
#include "Component.h"
#include "MyVmath.h"
#include "Collider.h"
#include "MeshRenderer.h"

#define NUM_COLOURS 3

class ColourOnRayHit : public Component
{
public:

   ColourOnRayHit();

   void EditUpdate() override;
   int32_t GetType() override;

protected:
   void EditAwake( IEditor& editor ) override;

private:

   vmath::vec4 m_Colours[ NUM_COLOURS ];
   Collider* m_MyCollider;
   MeshRenderer* m_MyMeshRen;
   int m_NextColour;
};
