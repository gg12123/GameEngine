#include <float.h>
#include "Physics.h"
#include "Collider.h"

Physics::Physics()
{
}

std::list<Collider*>::iterator Physics::RegisterCollider( Collider& col )
{
   m_Colliders.push_front( &col );
   return m_Colliders.begin();
}

void Physics::UnRegisterCollider( const std::list<Collider*>::iterator toRemove )
{
   m_Colliders.erase( toRemove );
}

bool Physics::RayCast( const Ray& ray, RayCastHit& hit )
{
   RayCastHit testHit;
   bool rayHit = false;

   hit.Distance = FLT_MAX;

   for (auto it = m_Colliders.begin(); it != m_Colliders.end(); it++)
   {
      if ((*it)->IsCollidedWithRay( ray, testHit ))
      {
         if (testHit.Distance < hit.Distance)
         {
            hit = testHit;
            rayHit = true;
         }
      }
   }
   return rayHit;
}