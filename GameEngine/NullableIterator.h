#pragma once

template<typename T>
class NullableValue
{
public:

   NullableValue()
   {
      m_IsNull = true;
   }

   T Get()
   {
      if (m_IsNull)
      {
         throw std::exception( "Value is not set" );
      }

      return m_Value;
   }

   void Set( T val )
   {
      m_Value = val;
      m_IsNull = false;
   }

   void Clear()
   {
      m_IsNull = true;
   }

   bool IsNull()
   {
      return m_IsNull;
   }

private:
   T m_Value;
   bool m_IsNull;
};
