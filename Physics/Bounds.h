#pragma once
#include "GridDirections.h"

class Bounds
{
public:
   float GetBound( const GridDirection dir ) const
   {
      return m_Bounds[ static_cast<int>(dir) ];
   }

   void SetBound( const GridDirection dir, const float val )
   {
      m_Bounds[ static_cast<int>(dir) ] = val;
   }

private:
   float m_Bounds[ NUMBER_OF_GRID_DIRS ];
};
