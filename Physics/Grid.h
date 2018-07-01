#pragma once
#include "GridPosition.h"
#include "ThreeDArray.h"

class GridSquare;

class Grid
{
public:

   constexpr static int Size = 100;

   GridSquare& GetSquare( const GridPosition& pos )
   {
      return *m_Squares.Get( pos.XIndex, pos.YIndex, pos.ZIndex );
   }

private:
   ThreeDArray<GridSquare*, Size, Size, Size> m_Squares;
};
