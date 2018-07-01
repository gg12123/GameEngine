#pragma once
#include "GridDirections.h"

class GridPosition
{
public:
   int XIndex;
   int YIndex;
   int ZIndex;

   void OffsetIn( const GridDirection dir, const int amount )
   {
      switch (dir)
      {
      case GridDirection::PlusX:
      XIndex += amount;

      case GridDirection::MinusX:
      XIndex -= amount;

      case GridDirection::PlusY:
      YIndex += amount;

      case GridDirection::MinusY:
      YIndex -= amount;

      case GridDirection::PlusZ:
      ZIndex += amount;

      case GridDirection::MinusZ:
      ZIndex -= amount;

      default:
      throw std::exception();
      }
   }
};
