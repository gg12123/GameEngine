#pragma once

class Shape;

class CollisionMatrix
{
public:
   bool IsAlreadyChecked( const Shape& shape1, const Shape& shape2 );

   void Clear( const Shape& shape1, const Shape& shape2 );
   void Set( const Shape& shape1, const Shape& shape2 );
};
