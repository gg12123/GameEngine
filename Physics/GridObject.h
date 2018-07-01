#pragma once
#include "GridDirections.h"
#include "GridObjectsPtrsToSelf.h"
#include "Bounds.h"
#include "GridEnumerator.h"
#include "GridSquare.h"

class Shape;

class GridObject
{
public:
   void Update();
   GridEnumerator& GetSqauresOccupied();

private:
   void InitEnumerator( const GridDirection dir );

   bool NeedExpandingIn( const GridDirection dir ) const;
   bool NeedShrinkingIn( const GridDirection dir ) const;

   void ExpandIn( const GridDirection dir );
   void ShrinkIn( const GridDirection dir );

   GridSquare& CalculateAnchor( const GridDirection normal ) const;
   int CalculateCount( const GridDirection indexAxis ) const;

   GridSquare& BoundingSquare( const GridDirection dir ) const;
   float Bound( const GridDirection dir ) const;

   void AddToSqaure( GridSquare& square );
   void RemoveFromSqaure( GridSquare& square );

   GridSquare* m_BoundingSquares[ NUMBER_OF_GRID_DIRS ];
   GridObjectsPtrsToSelf<5> m_PtrsToSelf;
   GridEnumerator m_Enumerator;
   Shape* m_Shape;
};
