#pragma once
#include "Bounds.h"
#include "GridDirections.h"

class Shape
{
public:
   bool IsCollidedWith( const Shape& other ) const
   {
      if (BoundingSpheresOverlap( other ))
      {

      }
   }

   float GetBound( const GridDirection dir ) const
   {
      return m_Bounds.GetBound( dir );
   }

   Bounds& GetBounds()
   {
      return m_Bounds;
   }

private:
   void UpdateBounds(); // call this each time the shape gets moved.
   bool BoundingSpheresOverlap( const Shape& other ) const;

   Bounds m_Bounds;
};
