#include <float.h>
#include "BoxCollider.h"
#include "Ray.h"
#include "GameObject.h"
#include "Transform.h"
#include "ComponentIDs.h"

bool BoxCollider::RayIsCollidingWithFace( const unsigned int depthAxis, const float depth, Ray& ray, RayCastHit& hit )
{
   const float p = ray.GetOrigin()[ depthAxis ];
   const float d = ray.GetDirection()[ depthAxis ];

   bool colliding = false;

   if (d != 0.0)
   {
      const float a = (depth - p) / d;
      vmath::vec3 Pc = ray.CalculateAt( a );

      const unsigned int uAxis = (depthAxis + 1) % 3;
      const unsigned int vAxis = (depthAxis + 2) % 3;

      colliding = ( Pc[ uAxis ] >= MinBound( uAxis ) && Pc[ uAxis ] <= MaxBound( uAxis ) &&
                    Pc[ vAxis ] >= MinBound( vAxis ) && Pc[ vAxis ] <= MaxBound( vAxis ) );

      if (colliding)
      {
         hit.Distance = length( ray.GetOrigin() - Pc );
         hit.CollisionPoint = Pc;
      }
   }

   return colliding;
}

int32_t BoxCollider::GetType()
{
   return COMPONENT_ID_BOXCOLLIDER;
}

bool BoxCollider::IsCollidedWithRay( const Ray& ray, RayCastHit& hit )
{
   bool collided = false;
   Ray localSpaceRay = ray.InLocalSpace( GetGameObject().GetTransform() );

   hit.Distance = FLT_MAX;

   RayCastHit testHit;

   for (unsigned int axis = 0; axis < 3; axis++)
   {
      if (RayIsCollidingWithFace( axis, MinBound( axis ), localSpaceRay, testHit ))
      {
         if (testHit.Distance < hit.Distance)
         {
            hit = testHit;
            collided = true;
         }
      }

      if (RayIsCollidingWithFace( axis, MaxBound( axis ), localSpaceRay, testHit ))
      {
         if (testHit.Distance < hit.Distance)
         {
            hit = testHit;
            collided = true;
         }
      }
   }

   if (collided)
   {
      hit.CollisionPoint = GetGameObject().GetTransform().ToGlobalPosition( hit.CollisionPoint );
      hit.HitCollider = this;
   }

   return collided;
}