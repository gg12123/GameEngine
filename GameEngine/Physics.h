#pragma once
#include <list>
#include "Ray.h"

class Collider;

class IPhysics
{
public:
   virtual bool RayCast( const Ray& ray, RayCastHit& hit ) = 0;
};

class Physics : public IPhysics
{
public:
   Physics();

   std::list<Collider*>::iterator RegisterCollider( Collider& col );
   void UnRegisterCollider( const std::list<Collider*>::iterator toRemove );

   bool RayCast( const Ray& ray, RayCastHit& hit ) override;

private:
   std::list<Collider*> m_Colliders;
};
