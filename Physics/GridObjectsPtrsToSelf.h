#pragma once
#include <assert.h>
#include "Container.h"
#include "ThreeDArray.h"

template<int MaxDimSize>
class GridObjectsPtrsToSelf
{
public:

   GridObjectsPtrsToSelf()
   {
      m_Elements.Init( nullptr );
   }

   Element& Take( const int x, const int y, const int z )
   {
      return m_Elements.GetAndReplace( x % MaxDimSize, y % MaxDimSize, z % MaxDimSize, nullptr);
   }

   void Put( const int x, const int y, const int z, Element& newElemnt )
   {
      m_Elements.Set( x % MaxDimSize, y % MaxDimSize, z % MaxDimSize, &newElemnt )
   }

private:
   ThreeDArray<Element*, MaxDimSize, MaxDimSize, MaxDimSize> m_Elements;
};
