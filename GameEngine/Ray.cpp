#include "Ray.h"
#include "Transform.h"

Ray::Ray( vmath::vec3 direction, vmath::vec3 origin )
{
   m_Direction = direction;
   m_Origin = origin;
}

Ray Ray::InLocalSpace( Transform& transform ) const
{
   Ray out( transform.ToLocalDirection( m_Direction ), transform.ToLocalPosition( m_Origin ) );
   return out;
}

vmath::vec3& Ray::GetOrigin()
{
   return m_Origin;
}

vmath::vec3& Ray::GetDirection()
{
   return m_Direction;
}

vmath::vec3 Ray::CalculateAt( const float u ) const
{
   return m_Origin + u * m_Direction;
}