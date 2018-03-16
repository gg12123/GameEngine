#pragma once
#include <vector>
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

   virtual void OnGUI( std::string fieldName, ISerializedFieldOwner& owner );
   virtual void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner );
   virtual void OnGUIClose();

   virtual void CopyFrom( SerializedField& toCopy ) = 0;

   virtual vmath::vec3 Vector3Value();
   virtual vmath::mat4 MatrixValue();
   virtual std::string StringValue();
   virtual vmath::vec4 Vector4Value();
   virtual float FloatValue();
   virtual int32_t IntValue();
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
   vmath::vec3 Vector3Value() override;
   void CopyFrom( SerializedField& toCopy ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   vmath::vec3 m_Value;
};

class SerializedVector4 : public FixedSizeSerializedField
{
public:
   vmath::vec4 Value();
   void SetValue( vmath::vec4 value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   vmath::vec4 Vector4Value() override;
   void CopyFrom( SerializedField& toCopy ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   vmath::vec4 m_Value;
};

class SerializedRotation : public FixedSizeSerializedField
{
public:
   vmath::mat4 Value();
   void SetValue( vmath::mat4 value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   vmath::mat4 MatrixValue() override;
   void CopyFrom( SerializedField& toCopy ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   vmath::mat4 m_Value;
   vmath::vec3 m_Euler;
};

#define MAX_SERAIALIZED_STRING_SIZE 50

class SerializedString : public SerializedField
{
public:
   std::string Value();
   void SetValue( std::string value );
   std::string StringValue() override;
   void CopyFrom( SerializedField& toCopy ) override;
protected:
   void DeSerializeWithSize( std::ifstream& stream ) override;
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   std::string m_Value;
};

class TypeInString : public SerializedString
{
public:
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
private:
   char m_Buffer[ MAX_SERAIALIZED_STRING_SIZE ];
};

class SelectableString : public SerializedString
{
public:
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   void InitForGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   void OnGUIClose() override;
protected:
   virtual std::string GetPathToSelectables() = 0;
private:
   std::vector<std::string> m_Selectables;
};

class ShaderField : public SelectableString
{
protected:
   std::string GetPathToSelectables() override;
};

class SerializedFloat : public FixedSizeSerializedField
{
public:
   float Value();
   void SetValue( float value );
   void OnGUI( std::string fieldName, ISerializedFieldOwner& owner ) override;
   float FloatValue() override;
   void CopyFrom( SerializedField& toCopy ) override;
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
   int32_t IntValue() override;
   void CopyFrom( SerializedField& toCopy ) override;
protected:
   void LocalSerialize( std::ofstream& stream ) override;
   void LocalDeSerialize( std::ifstream& stream ) override;
   int32_t GetSize() override;
private:
   int32_t m_Value;
};
