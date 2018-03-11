#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
   bool IsCollidedWithRay( const Ray& ray, RayCastHit& hit ) override;
   int32_t GetType() override;

private:
   bool RayIsCollidingWithFace( const unsigned int depthAxis, const float depth, const Ray& ray, RayCastHit& hit );
};
