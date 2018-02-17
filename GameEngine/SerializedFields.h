#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "vmath.h"

class SerializedField
{
public:
   virtual void Serialize( std::ofstream& stream ) = 0;
   virtual void DeSerialize( std::ifstream& stream ) = 0;
};

class SerializedVector3 : public SerializedField
{
public:
   vec3 Value();
   void SetValue( vec3 value );
private:
   vec3 m_Value;
};

class SerializedRotation : public SerializedField
{
public:
   mat4 Value();
   void SetValue( mat4 value );
private:
   mat4 m_Value;
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

class SerializedInt32 : public SerializedField
{
public:
   int32_t Value();
   void SetValue( int32_t value );
   void Serialize( std::ofstream& stream ) override;
   void DeSerialize( std::ifstream& stream ) override;
private:
   int32_t m_Value;
};
