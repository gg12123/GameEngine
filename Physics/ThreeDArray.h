#pragma once

template<class T, int X, int Y, int Z>
class ThreeDArray
{
public:

   ThreeDArray()
   {
      m_XY = X * Y;
   }

   void Init( T inital )
   {
      for (int i = 0; i < NumValuesNeeded; i++)
         m_Values[ i ] = inital;
   }

   T Get( const int x, const int y, const int z )
   {
      return m_Values[ GetIndex( x, y, z ) ];
   }

   T GetAndReplace( const int x, const int y, const int z, T newVal )
   {
      auto i = GetIndex( x, y, z );

      auto val = m_Values[ i ];
      m_Values[ i ] = newVal;

      return val;
   }

   void Set( const int x, const int y, const int z, T val )
   {
      m_Values[ GetIndex( x, y, z ) ] = val;
   }

private:
   static constexpr int NumValuesNeeded = X * Y * Z;

   int GetIndex( const int x, const int y, const int z )
   {
      return z * m_XY + y * X + x;
   }

   T m_Values[ NumValuesNeeded ];
   int m_XY;
};
