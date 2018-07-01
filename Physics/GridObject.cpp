#include <math.h>
#include "GridObject.h"
#include "Shape.h"

GridObject::GridObject()
{
}

bool GridObject::NeedExpandingIn( const GridDirection dir ) const
{
   return (Bound( dir ) > BoundingSquare( dir ).GetBound( dir ));
}

bool GridObject::NeedShrinkingIn( const GridDirection dir ) const
{
   return (Bound( dir ) < BoundingSquare( dir ).GetBound( GetOpposite( dir ) ));
}

GridEnumerator& GridObject::GetSqauresOccupied()
{
   auto xCount = fabs( BoundingSquare( GridDirection::PlusX ).GetIndex( GridDirection::PlusX ) -
                      BoundingSquare( GridDirection::MinusX ).GetIndex( GridDirection::PlusX ) ) + 1;

   auto yCount = fabs( BoundingSquare( GridDirection::PlusY ).GetIndex( GridDirection::PlusY ) -
                      BoundingSquare( GridDirection::MinusY ).GetIndex( GridDirection::PlusY ) ) + 1;

   auto zCount = fabs( BoundingSquare( GridDirection::PlusZ ).GetIndex( GridDirection::PlusZ ) -
                      BoundingSquare( GridDirection::MinusZ ).GetIndex( GridDirection::PlusZ ) ) + 1;

   auto yDiff = BoundingSquare( GridDirection::MinusX ).GetIndex( GridDirection::PlusY ) -
      BoundingSquare( GridDirection::MinusY ).GetIndex( GridDirection::PlusY );

   auto zDiff = BoundingSquare( GridDirection::MinusX ).GetIndex( GridDirection::PlusZ ) -
      BoundingSquare( GridDirection::MinusZ ).GetIndex( GridDirection::PlusZ );

   auto& anchor = BoundingSquare( GridDirection::MinusX ).
      GetNext( GridDirection::MinusY, GridDirection::MinusZ, yDiff, zDiff );

   m_Enumerator.Init( anchor, xCount, yCount, zCount,
                      GridDirection::PlusX, GridDirection::PlusY, GridDirection::PlusZ );

   return m_Enumerator;
}

void GridObject::Update()
{
   for (int i = 0; i < NUMBER_OF_GRID_DIRS; i++)
   {
      auto dir = static_cast<GridDirection>(i);

      if (NeedExpandingIn( dir ))
      {
         ExpandIn( dir );
      }
      else if (NeedShrinkingIn( dir ))
      {
         ShrinkIn( dir );
      }
   }
}

void GridObject::InitEnumerator( const GridDirection normal )
{
   auto& anchor = CalculateAnchor( normal );

   auto perp1 = GetPerp1( normal );
   auto perp2 = GetPerp2( normal );

   auto count1 = CalculateCount( perp1 );
   auto count2 = CalculateCount( perp2 );

   m_Enumerator.Init( anchor, count1, count2, 1, perp1, perp2, normal );
}

void GridObject::ShrinkIn( const GridDirection dir )
{
   InitEnumerator( dir );

   for (auto it = m_Enumerator.First(); it != m_Enumerator.End(); it = m_Enumerator.Next())
   {
      RemoveFromSqaure( *it );
   }

   auto& oldBound = BoundingSquare( dir );
   m_BoundingSquares[ static_cast<int>(dir) ] = &oldBound.GetNext( GetOpposite( dir ) );
}

void GridObject::ExpandIn( const GridDirection dir )
{
   auto& oldBound = BoundingSquare( dir );
   m_BoundingSquares[ static_cast<int>(dir) ] = &oldBound.GetNext( dir );

   InitEnumerator( dir );

   for (auto it = m_Enumerator.First(); it != m_Enumerator.End(); it = m_Enumerator.Next())
   {
      AddToSqaure( *it );
   }
}

GridSquare& GridObject::CalculateAnchor( const GridDirection normal ) const
{
   auto xDir = GetPerp1( normal );
   auto yDir = GetPerp2( normal );

   auto& centre = BoundingSquare( normal );

   auto x = BoundingSquare(xDir).GetIndex( xDir ) - centre.GetIndex( xDir );
   auto y = BoundingSquare(yDir).GetIndex( yDir ) - centre.GetIndex( yDir );

   return centre.GetNext( xDir, yDir, x, y );
}

int GridObject::CalculateCount( const GridDirection axis ) const
{
   auto minusAxis = GetOpposite( axis );
   return fabs(BoundingSquare( axis ).GetIndex( axis ) - BoundingSquare( minusAxis ).GetIndex( axis )) + 1;
}

void GridObject::AddToSqaure( GridSquare& square )
{
   auto e = square.Add( *m_Shape );

   m_PtrsToSelf.Put( square.GetIndex( GridDirection::PlusX ),
                     square.GetIndex( GridDirection::PlusY ),
                     square.GetIndex( GridDirection::PlusZ ),
                     e );
}

void GridObject::RemoveFromSqaure( GridSquare& square )
{
   auto& e = m_PtrsToSelf.Take( square.GetIndex( GridDirection::PlusX ),
                                square.GetIndex( GridDirection::PlusY ),
                                square.GetIndex( GridDirection::PlusZ ) );

   square.Remove( e );
}

GridSquare& GridObject::BoundingSquare( const GridDirection dir ) const
{
   return *m_BoundingSquares[ static_cast<int>(dir) ];
}

float GridObject::Bound( const GridDirection dir ) const
{
   return m_Shape->GetBound( dir );
}