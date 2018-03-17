#pragma once
#include "MyVmath.h"

class Transform;
class Collider;

class Ray
{
public:
   Ray( vmath::vec3 direction, vmath::vec3 origin );

   Ray InLocalSpace( Transform& transform ) const;

   vmath::vec3& GetOrigin();
   vmath::vec3& GetDirection();

   vmath::vec3 CalculateAt( const float u ) const;

private:
   vmath::vec3 m_Direction;
   vmath::vec3 m_Origin;
};

struct RayCastHit
{
   float Distance;
   vmath::vec3 CollisionPoint;
   Collider* HitCollider;
};