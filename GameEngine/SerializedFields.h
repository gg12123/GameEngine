#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "MyVmath.h"

class ISerializedFieldOwner;

class SerializedField
{
public:
   void Serialize( std::ofstream& stream );
   void DeSerialize( std::ifstream& stream );

   void SerializeWithSize( std::ofstream& stream );
   virtual void DeSerializeWithSize( std::ifstream& stream ) = 0;

   virtual void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) = 0;
   virtual void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner );
protected:
   virtual void LocalSerialize( std::ofstream& stream ) = 0;
   virtual void LocalDeSerialize( std::ifstream& stream ) = 0;
   virtual int32_t GetSize() = 0;
};

class FixedSizeSerializedField : public SerializedField
{
public:
   void DeSerializeWithSize( std::ifstream& stream ) override;
};

class SerializedVector3 : public FixedSizeSerializedField
{
public:
   vmath::vec3 Value();
   void SetValue( vmath::vec3 value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   vmath::vec3 m_Value;
   std::string m_ElementNames[ 3 ];
};

class SerializedRotation : public FixedSizeSerializedField
{
public:
   vmath::mat4 Value();
   void SetValue( vmath::mat4 value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   vmath::mat4 m_Value;
   vmath::vec3 m_Euler;
};

class SerializedString : public SerializedField
{
public:
   std::string Value();
   void SetValue( std::string value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
protected:
   void DeSerializeWithSize( std::ifstream& stream ) override;
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   std::string m_Value;
};

class SerializedFloat : public FixedSizeSerializedField
{
public:
   float Value();
   void SetValue( float value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   float m_Value;
};

class SerializedInt32 : public FixedSizeSerializedField
{
public:
   int32_t Value();
   void SetValue( int32_t value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   int32_t m_Value;
};
