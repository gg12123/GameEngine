#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "vmath.h"

class SerializedField
{
public:
   void Serialize( std::ofstream& stream );
   void DeSerialize( std::ifstream& stream );

   void SerializeWithSize( std::ofstream& stream );
   void DeSerializeWithSize( std::ifstream& stream );
protected:
   virtual void LocalSerialize( std::ofstream& stream ) = 0;
   virtual void LocalDeSerialize( std::ifstream& stream ) = 0;
   virtual int32_t GetSize() = 0;
};

class SerializedVector3 : public SerializedField
{
public:
   vec3 Value();
   void SetValue( vec3 value );
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   vec3 m_Value;
};

class SerializedRotation : public SerializedField
{
public:
   mat4 Value();
   void SetValue( mat4 value );
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   mat4 m_Value;
};

class SerializedString : public SerializedField
{
public:
   std::string Value();
   void SetValue( std::string value );
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   std::string m_Value;
};

class SerializedFloat : public SerializedField
{
public:
   float Value();
   void SetValue( float value );
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   float m_Value;
};

class SerializedInt32 : public SerializedField
{
public:
   int32_t Value();
   void SetValue( int32_t value );
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   int32_t m_Value;
};
