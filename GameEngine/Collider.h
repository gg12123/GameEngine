#pragma once
#include "Component.h"
#include "MyVmath.h"

class Ray;
class MeshBounds;
class RayCastHit;

class Collider : public Component
{
public:
   Collider();

   virtual bool IsCollidedWithRay( const Ray& ray, RayCastHit& hit ) = 0;

protected:
   // register with the physics engine
   void Awake() override;
   void EditAwake( IEditor& editor ) override;

   // use mesh bounds and transform scale to find bounds for each axis
   float CalculateMinBound( unsigned int axis );
   float CalculateGetMaxBound( unsigned int axis );
   void CacheScale();

private:
   MeshBounds* m_MeshBounds;
   vmath::vec3 m_Scale;
};
