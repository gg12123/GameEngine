#pragma once
#include <memory>

class Element
{
public:
   Element( int index )
   {
      Index = index;
   }

   int Index;
};

template<class T, int Size>
class Container
{
public:

   Container()
   {
      for (int i = 0; i < Size; i++)
      {
         m_Pointers[ i ] = std::unique_ptr<Element>( new Element( i ) );
      }
   }

   Element* Add( T toAdd )
   {
      m_Values[ m_Count ] = toAdd;
      m_Count++;
      return m_Pointers[ m_Count - 1 ].get();
   }

   void Remove( Element* toRemove )
   {
      auto newFree = toRemove->Index;
      auto end = m_Count - 1;

      // move the unique ptr from the new slot
      auto toRemoveUPtr = m_Pointers[ newFree ];

      // Move end pointer to the free slot
      m_Pointers[ newFree ] = m_Pointers[ end ];
      m_Pointers[ newFree ]->Index = newFree;

      // Move end value
      m_Values[ newFree ] = m_Values[ end ];

      // Put the old pointer at the end
      m_Pointers[ end ] = toRemoveUPtr;
      m_Pointers[ end ]->Index = end;

      m_Count--;
   }

   T* Values()
   {
      return m_Values;
   }

   int Count()
   {
      return m_Count;
   }

private:
   T m_Values[ Size ];
   std::unique_ptr<Element> m_Pointers[ Size ];
   int m_Count;
};
