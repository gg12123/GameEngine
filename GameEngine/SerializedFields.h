#pragma once
#include <string>
#include "vmath.h"
#include "Quaternion.h"

class SerializedField
{
};

class SerializedVector3 : public SerializedField
{
public:
   vec3 Value();
   void SetValue( vec3 value );
private:
   vec3 m_Value;
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

class SerializedFloat : public SerializedField
{
public:
   float Value();
   void SetValue( float value );
private:
   float m_Value;
};
