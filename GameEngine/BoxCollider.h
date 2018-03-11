#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
   bool IsCollidedWithRay( const Ray& ray, RayCastHit& hit ) override;

private:
   bool RayIsCollidingAlongAxis( const unsigned int axis, const Ray& ray, RayCastHit& hit );
};
