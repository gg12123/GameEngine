#pragma once
#include "GridSquare.h"
#include "GridDirections.h"

class GridEnumerator
{
public:

   void Init(GridSquare& anchor, const int count1, const int count2, const int count3,
              const GridDirection dir1, const GridDirection dir2, const GridDirection dir3)
   {
      m_Anchor = &anchor;

      m_Count1 = count1;
      m_Count2 = count2;
      m_Count3 = count3;

      m_Dir1 = dir1;
      m_Dir2 = dir2;
      m_Dir3 = dir3;

      m_Curr1 = 0;
      m_Curr2 = 0;
      m_Curr3 = 0;
   }

   GridSquare* Next()
   {
      m_Curr1++;

      if (m_Curr1 >= m_Count1)
      {
         m_Curr1 = 0;
         m_Curr2++;
      }

      if (m_Curr2 >= m_Count2)
      {
         m_Curr2 = 0;
         m_Curr3++;
      }

      if (m_Curr3 >= m_Count3)
         return nullptr;
       
      return &m_Anchor->GetNext( m_Dir1, m_Dir2, m_Dir3, m_Curr1, m_Curr2, m_Curr3 );
   }

   GridSquare* First()
   {
      return m_Anchor;
   }

   GridSquare* End()
   {
      return nullptr;
   }

private:
   GridSquare* m_Anchor = nullptr;

   GridDirection m_Dir1;
   GridDirection m_Dir2;
   GridDirection m_Dir3;

   int m_Count1;
   int m_Count2;
   int m_Count3;

   int m_Curr1;
   int m_Curr2;
   int m_Curr3;
};
