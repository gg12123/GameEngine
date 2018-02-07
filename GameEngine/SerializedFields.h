#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class SerializedField
{
};

class SerializedVector3 : SerializedField
{
public:
   Vector3 Value();
   void SetValue( Vector3 value );
private:
   Vector3 m_Value;
};

class SerializedQuaternion : SerializedField
{
public:
   Quaternion Value();
   void SetValue( Quaternion value );
private:
   Quaternion m_Value;
};
