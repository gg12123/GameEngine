#pragma once
#include <string>
#include "Vector3.h"
#include "Quaternion.h"

class SerializedField
{
};

class SerializedVector3 : public SerializedField
{
public:
   Vector3 Value();
   void SetValue( Vector3 value );
private:
   Vector3 m_Value;
};

class SerializedQuaternion : public SerializedField
{
public:
   Quaternion Value();
   void SetValue( Quaternion value );
private:
   Quaternion m_Value;
};

class SerializedString : public SerializedField
{
public:
   std::string Value();
   void SetValue( std::string value );
private:
   std::string m_Value;
};
