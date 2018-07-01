#pragma once
#include "GridDirections.h"
#include "Container.h"
#include "Bounds.h"
#include "GridPosition.h"

class GridObject;
class Shape;
class Grid;

class GridSquare
{
public:
   static constexpr int MaxObjectCount = 10;

   GridSquare& GetNext( const GridDirection dir ) const;
   GridSquare& GetNext( const GridDirection dir, const int numAlong ) const;
   GridSquare& GetNext( const GridDirection dir1, const GridDirection dir2,
                        const int numAlong1, const int numAlong2 ) const;
   GridSquare& GetNext( const GridDirection dir1, const GridDirection dir2, const GridDirection dir3,
                        const int numAlong1, const int numAlong2, const int numAlong3 ) const;

   float GetBound( const GridDirection dir ) const;
   int GetIndex( const GridDirection dir ) const;

   Element& Add( Shape& toAdd );
   void Remove( Element& toRemove );

   auto& GetShapes()
   {
      return m_Shapes;
   }

private:
   Bounds m_Bounds;
   Container<Shape*, MaxObjectCount> m_Shapes;
   Grid* m_Grid;
   GridPosition m_Position;
};
