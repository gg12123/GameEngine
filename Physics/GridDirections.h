#pragma once
#include <exception>

#define NUMBER_OF_GRID_DIRS 6

enum class GridDirection
{
   PlusX,
   MinusX,
   PlusY,
   MinusY,
   PlusZ,
   MinusZ
};

inline GridDirection GetOpposite( const GridDirection dir )
{
   switch (dir)
   {
   case GridDirection::PlusX:
   return GridDirection::MinusX;

   case GridDirection::MinusX:
   return GridDirection::PlusX;

   case GridDirection::PlusY:
   return GridDirection::MinusY;

   case GridDirection::MinusY:
   return GridDirection::PlusY;

   case GridDirection::PlusZ:
   return GridDirection::MinusZ;

   case GridDirection::MinusZ:
   return GridDirection::PlusZ;

   default:
   throw std::exception();
   }
}

inline GridDirection GetPerp1( const GridDirection normal )
{
   switch (normal)
   {
   case GridDirection::PlusX:
   case GridDirection::MinusX:
   return GridDirection::PlusY;

   case GridDirection::PlusY:
   case GridDirection::MinusY:
   return GridDirection::PlusX;

   case GridDirection::PlusZ:
   case GridDirection::MinusZ:
   return GridDirection::PlusY;

   default:
   throw std::exception();
   }
}

inline GridDirection GetPerp2( const GridDirection normal )
{
   switch (normal)
   {
   case GridDirection::PlusX:
   case GridDirection::MinusX:
   return GridDirection::PlusZ;

   case GridDirection::PlusY:
   case GridDirection::MinusY:
   return GridDirection::PlusZ;

   case GridDirection::PlusZ:
   case GridDirection::MinusZ:
   return GridDirection::PlusX;

   default:
   throw std::exception();
   }
}