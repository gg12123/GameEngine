#pragma once
#include "GridObject.h"
#include "Shape.h"
#include "CollisionMatrix.h"

class CollisionDetector
{
public:
   void Detect()
   {
      auto squares = m_MyGridObj->GetSqauresOccupied();

      for (auto sq = squares.First(); sq != squares.End(); sq = squares.Next())
      {
         auto shapes = sq->GetShapes().Values();
         auto shapeCount = sq->GetShapes().Count();

         for (int i = 0; i < shapeCount; i++)
         {
            DoDetection( *shapes[i] );
         }
      }
   }

private:
   void DoDetection( Shape& shape )
   {
      if (&shape != m_MyShape)
      {
         if (m_Matrix->IsAlreadyChecked( *m_MyShape, shape ))
         {
            m_Matrix->Clear( *m_MyShape, shape );
         }
         else
         {
            if (m_MyShape->IsCollidedWith( shape ))
            {
               // log the collision
            }

            m_Matrix->Set( *m_MyShape, shape );
         }
      }
   }

   Shape* m_MyShape = nullptr;
   GridObject* m_MyGridObj = nullptr;
   CollisionMatrix* m_Matrix = nullptr;
};
