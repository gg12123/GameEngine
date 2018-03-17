#pragma once
#include <list>
#include "Ray.h"

class Collider;

class Physics
{
public:
   Physics();

   std::list<Collider*>::iterator RegisterCollider( Collider& col );
   void UnRegisterCollider( const std::list<Collider*>::iterator toRemove );

   bool RayCast( const Ray& ray, RayCastHit& hit );

private:
   std::list<Collider*> m_Colliders;
};
