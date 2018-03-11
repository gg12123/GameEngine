#include <float.h>
#include "BoxCollider.h"
#include "Ray.h"
#include "GameObject.h"
#include "Transform.h"
#include "ComponentIDs.h"

bool BoxCollider::RayIsCollidingWithFace( const unsigned int depthAxis, const float depth, const Ray& ray, RayCastHit& hit )
{
   const float p = ray.GetOrigin()[ depthAxis ];
   const float d = ray.GetDirection()[ depthAxis ];

   bool colliding = false;

   if (d != 0.0)
   {
      const float a = (depth - p) / d;
      vmath::vec3 Pc = ray.GetOrigin() + a * ray.GetDirection();

      const unsigned int uAxis = (depthAxis + 1) % 3;
      const unsigned int vAxis = (depthAxis + 2) % 3;

      colliding = ( Pc[ uAxis ] >= CalculateMinBound( uAxis ) && Pc[ uAxis ] <= CalculateMaxBound( uAxis ) &&
                    Pc[ vAxis ] >= CalculateMinBound( vAxis ) && Pc[ vAxis ] <= CalculateMaxBound( vAxis ) );

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
   CacheScale();
   bool collided = false;
   Ray localSpaceRay = ray.InLocalSpace( GetGameObject().GetTransform() );

   hit.Distance = FLT_MAX;

   RayCastHit testHit;

   for (unsigned int axis = 0; axis < 3; axis++)
   {
      if (RayIsCollidingWithFace( axis, CalculateMinBound( axis ), localSpaceRay, testHit ))
      {
         if (testHit.Distance < hit.Distance)
         {
            hit = testHit;
            collided = true;
         }
      }

      if (RayIsCollidingWithFace( axis, CalculateMaxBound( axis ), localSpaceRay, testHit ))
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
   }
}