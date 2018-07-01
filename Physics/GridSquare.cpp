#include <assert.h>
#include "GridSquare.h"
#include "Grid.h"

GridSquare& GridSquare::GetNext( const GridDirection dir1, const GridDirection dir2, const GridDirection dir3,
                                 const int numAlong1, const int numAlong2, const int numAlong3 ) const
{
   auto pos = m_Position;

   pos.OffsetIn( dir1, numAlong1 );
   pos.OffsetIn( dir2, numAlong2 );
   pos.OffsetIn( dir3, numAlong3 );

   return m_Grid->GetSquare( pos );
}

GridSquare& GridSquare::GetNext( const GridDirection dir1, const GridDirection dir2,
                                 const int numAlong1, const int numAlong2 ) const
{
   return GetNext( dir1, dir2, dir2, numAlong1, numAlong2, 0 );
}

GridSquare& GridSquare::GetNext( const GridDirection dir, const int numAlong ) const
{
   return GetNext( dir, dir, dir, numAlong, 0, 0 );
}

GridSquare& GridSquare::GetNext( const GridDirection dir ) const
{
   return GetNext( dir, dir, dir, 1, 0, 0 );
}

Element& GridSquare::Add( Shape& toAdd )
{
   return *m_Shapes.Add( &toAdd );
}

void GridSquare::Remove( Element& toRemove )
{
   m_Shapes.Remove( &toRemove );
}

float GridSquare::GetBound( const GridDirection dir ) const
{
   m_Bounds.GetBound( dir );
}

int GridSquare::GetIndex( const GridDirection dir ) const
{
   switch (dir)
   {
   case GridDirection::PlusX:
   case GridDirection::MinusX:
   return m_Position.XIndex;

   case GridDirection::PlusY:
   case GridDirection::MinusY:
   return m_Position.YIndex;

   case GridDirection::PlusZ:
   case GridDirection::MinusZ:
   return m_Position.ZIndex;

   default:
   assert( false );
   }
}