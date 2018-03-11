#include "BoxCollider.h"
#include "Ray.h"
#include "GameObject.h"

bool BoxCollider::RayIsCollidingAlongAxis( const unsigned int axis, const Ray& ray, RayCastHit& hit )
{

}

bool BoxCollider::IsCollidedWithRay( const Ray& ray, RayCastHit& hit )
{
   CacheScale();
   bool collided = false;
   Ray localSpaceRay = ray.InLocalSpace( GetGameObject().GetTransform() );

   hit.Distance = MAX_FLOAT;

   RayCastHit testHit;

   for (unsigned int axis = 0; axis < 3; axis++)
   {
      if (RayIsCollidingAlongAxis( axis, localSpaceRay, testHit ))
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
      // change hit.CollisionPoint to world space
   }
}