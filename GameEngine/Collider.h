#pragma once
#include "Component.h"
#include "MyVmath.h"

class Ray;
class MeshBounds;
struct RayCastHit;

class Collider : public Component
{
public:
   Collider();

   virtual bool IsCollidedWithRay( const Ray& ray, RayCastHit& hit ) = 0;

   void ReCalculateBounds();

protected:
   // register with the physics engine
   void Awake() override;
   void EditAwake( IEditor& editor ) override;

   // use mesh bounds and transform scale to find bounds for each axis
   float MinBound( unsigned int axis );
   float MaxBound( unsigned int axis );

private:
   void TryCacheMeshBounds();

   MeshBounds* m_MeshBounds;
   vmath::vec2 m_Bounds[ 3 ];
};
