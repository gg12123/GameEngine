#include "SerializedFields.h"
#include "Debug.h"
#include "ImGUI/imgui.h"
#include "ISearializedFieldOwner.h"
#include "Utils.h"
#include "Path.h"

// ################ BASE #######################

void SerializedField::Serialize( std::ofstream& stream )
{
   LocalSerialize( stream );
}

void SerializedField::DeSerialize( std::ifstream& stream )
{
   LocalDeSerialize( stream );
}

void SerializedField::SerializeWithSize( std::ofstream& stream )
{
   SerializedInt32 size;
   size.SetValue( GetSize() );
   size.Serialize( stream );

   LocalSerialize( stream );
}

void SerializedField::InitForGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
}

void SerializedField::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
}

void SerializedField::OnGUIClose()
{
}

vmath::vec3 SerializedField::Vector3Value()
{
   throw std::exception( "Serailized field not a vector3" );
}

vmath::vec4 SerializedField::Vector4Value()
{
   throw std::exception( "Serailized field not a vector4" );
}

vmath::mat4 SerializedField::MatrixValue()
{
   throw std::exception( "Serailized field not a matrix" );
}

std::string SerializedField::StringValue()
{
   throw std::exception( "Serailized field not a string" );
}

float SerializedField::FloatValue()
{
   throw std::exception( "Serailized field not a float" );
}

int32_t SerializedField::IntValue()
{
   throw std::exception( "Serailized field not an int" );
}

// ################### FIXED SIZE ####################################

void FixedSizeSerializedField::DeSerializeWithSize( std::ifstream& stream )
{
   SerializedInt32 size;
   size.DeSerialize( stream );

   if (size.Value() != GetSize())
   {
      Debug::Instance().LogError( "Size inconsistency in serialized field" );
   }

   LocalDeSerialize( stream );
}

// ####################### VECTOR ########################### 

void SerializedVector::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::Text( fieldName.c_str() );

   for (int i = 0; i < GetCount(); i++)
   {
      if (ImGui::InputFloat( (fieldName + GetNameAt( i )).c_str(), GetPtrAt( i ) ))
      {
         owner.OnNewSerializedFields();
      }
   }
}

void SerializedVector::LocalSerialize( std::ofstream& stream )
{
   SerializedFloat f;

   for (int i = 0; i < GetCount(); i++)
   {
      f.SetValue( *GetPtrAt( i ) );
      f.Serialize( stream );
   }
}

void SerializedVector::LocalDeSerialize( std::ifstream& stream )
{
   SerializedFloat f;

   for (int i = 0; i < GetCount(); i++)
   {
      f.DeSerialize( stream );
      *GetPtrAt( i ) = f.Value();
   }
}

int32_t SerializedVector::GetSize()
{
   return GetCount() * sizeof( float );
}

// ####################### VECTOR3 ########################### 

vmath::vec3 SerializedVector3::Value()
{
   return m_Value;
}

void SerializedVector3::SetValue( vmath::vec3 value )
{
   m_Value = value;
}

vmath::vec3 SerializedVector3::Vector3Value()
{
   return m_Value;
}

void SerializedVector3::CopyFrom( SerializedField& toCopy )
{
   m_Value = toCopy.Vector3Value();
}

int SerializedVector3::GetCount()
{
   return 3;
}

std::string SerializedVector3::GetNameAt( int i )
{
   static std::string names[] = { " X", " Y", " Z" };
   return names[ i ];
}

float* SerializedVector3::GetPtrAt( int i )
{
   return &m_Value[ i ];
}

// ####################### VECTOR4 ########################### 

vmath::vec4 SerializedVector4::Value()
{
   return m_Value;
}

void SerializedVector4::SetValue( vmath::vec4 value )
{
   m_Value = value;
}

vmath::vec4 SerializedVector4::Vector4Value()
{
   return m_Value;
}

void SerializedVector4::CopyFrom( SerializedField& toCopy )
{
   m_Value = toCopy.Vector4Value();
}

int SerializedVector4::GetCount()
{
   return 4;
}

std::string SerializedVector4::GetNameAt( int i )
{
   static std::string names[] = { " R", " G", " B", " A" };
   return names[ i ];
}

float* SerializedVector4::GetPtrAt( int i )
{
   return &m_Value[ i ];
}

// ####################### ROTATION ########################### 

void SerializedRotation::InitForGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   m_Euler = vmath::matrixToEuler( m_Value );
}

void SerializedRotation::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::Text( fieldName.c_str() );

   bool inputDone = false;

   if (ImGui::Button( "Reset" ))
   {
      m_Euler = vmath::vec3( 0.0f, 0.0f, 0.0f );
      inputDone = true;
   }

   if (ImGui::SliderFloat( "Y (yaw)", &m_Euler[ 1 ], -180.0f, 180.0f ))
   {
      inputDone = true;
   }

   if (ImGui::SliderFloat( "X (pitch)", &m_Euler[ 0 ], -180.0f, 180.0f ))
   {
      inputDone = true;
   }

   if (ImGui::SliderFloat( "Z (roll)", &m_Euler[ 2 ], -180.0f, 180.0f ))
   {
      inputDone = true;
   }

   if (inputDone)
   {
      m_Value = vmath::eulerToMatrix( m_Euler[ 1 ], m_Euler[ 0 ], m_Euler[ 2 ] );
      owner.OnNewSerializedFields();
   }
}

vmath::mat4 SerializedRotation::Value()
{
   return m_Value;
}

void SerializedRotation::SetValue( vmath::mat4 value )
{
   m_Value = value;
}

void SerializedRotation::LocalSerialize( std::ofstream& stream )
{
   SerializedFloat f;

   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         f.SetValue( m_Value[ i ][ j ] );
         f.Serialize( stream );
      }
   }
}

void SerializedRotation::LocalDeSerialize( std::ifstream& stream )
{
   SerializedFloat f;

   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         f.DeSerialize( stream );
         m_Value[ i ][ j ] = f.Value();
      }
   }
}

int32_t SerializedRotation::GetSize()
{
   return 16 * sizeof( float );
}

vmath::mat4 SerializedRotation::MatrixValue()
{
   return m_Value;
}

void SerializedRotation::CopyFrom( SerializedField& toCopy )
{
   m_Value = toCopy.MatrixValue();
}

// ####################### FLOAT ########################### 

void SerializedFloat::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::InputFloat( fieldName.c_str(), &m_Value );
}

float SerializedFloat::Value()
{
   return m_Value;
}

void SerializedFloat::SetValue( float value )
{
   m_Value = value;
}

void SerializedFloat::LocalSerialize( std::ofstream& stream )
{
   stream.write( reinterpret_cast<const char*>(&m_Value), GetSize() );
}

void SerializedFloat::LocalDeSerialize( std::ifstream& stream )
{
   stream.read( reinterpret_cast<char*>(&m_Value), GetSize() );
}

int32_t SerializedFloat::GetSize()
{
   return sizeof( float );
}

float SerializedFloat::FloatValue()
{
   return m_Value;
}

void SerializedFloat::CopyFrom( SerializedField& toCopy )
{
   m_Value = toCopy.FloatValue();
}

// ####################### STRING ########################### 

std::string SerializedString::Value()
{
   return m_Value;
}

void SerializedString::SetValue( std::string value )
{
   m_Value = value;
}

void SerializedString::LocalSerialize( std::ofstream& stream )
{
   SerializedInt32 numChars;
   numChars.SetValue( m_Value.length() );
   numChars.Serialize( stream );
   
   stream.write( m_Value.c_str(), m_Value.size() );
}

void SerializedString::DeSerializeWithSize( std::ifstream& stream )
{
   SerializedInt32 x;

   x.DeSerialize( stream );
   int32_t fullSize = x.Value();

   x.DeSerialize( stream );
   int32_t numChars = x.Value();

   if (fullSize != (numChars * sizeof( char ) + sizeof( int32_t )))
   {
      Debug::Instance().LogError( "String size incorect" );
   }

   m_Value.resize( numChars );
   stream.read( &m_Value[ 0 ], numChars );
}

void SerializedString::LocalDeSerialize( std::ifstream& stream )
{
   SerializedInt32 numChars;
   numChars.DeSerialize( stream );

   m_Value.resize( numChars.Value() );

   stream.read( &m_Value[ 0 ], numChars.Value() );
}

int32_t SerializedString::GetSize()
{
   return m_Value.length() * sizeof( char ) + sizeof( int32_t );
}

std::string SerializedString::StringValue()
{
   return m_Value;
}

void SerializedString::CopyFrom( SerializedField& toCopy )
{
   m_Value = toCopy.StringValue();
}

void TypeInString::InitForGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   CopyStringToBuffer( m_Buffer, Value() );
}

void TypeInString::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::InputText( fieldName.c_str(), m_Buffer, MAX_SERAIALIZED_STRING_SIZE );

   ImGui::SameLine();
   if (ImGui::SmallButton( "OK" ))
   {
      SetValue( m_Buffer );
      owner.OnNewSerializedFields();
   }
}

void SelectableString::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::Text( fieldName.c_str() );
   ImGui::SameLine();

   if (ImGui::BeginMenu( Value().c_str() ))
   {
      for (auto it = m_Selectables.begin(); it != m_Selectables.end(); it++)
      {
         if (ImGui::Selectable( it->c_str() ))
         {
            SetValue( *it );
            owner.OnNewSerializedFields();
         }
      }

      ImGui::EndMenu();
   }
}

void SelectableString::InitForGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   m_Selectables.clear();
   GetFileNamesInDirectory( GetPathToSelectables(), m_Selectables );
}

void SelectableString::OnGUIClose()
{
   m_Selectables.clear();
}

std::string ShaderField::GetPathToSelectables()
{
   return Path::Instance().GetShaderPath( "" );
}

// ####################### INT32 ########################### 

void SerializedInt32::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::InputInt( fieldName.c_str(), &m_Value );
}

int32_t SerializedInt32::Value()
{
   return m_Value;
}

void SerializedInt32::SetValue( int32_t value )
{
   m_Value = value;
}

void SerializedInt32::LocalSerialize( std::ofstream& stream )
{
   stream.write( reinterpret_cast<const char*>(&m_Value), GetSize() );
}

void SerializedInt32::LocalDeSerialize( std::ifstream& stream )
{
   stream.read( reinterpret_cast<char*>(&m_Value), GetSize() );
}

int32_t SerializedInt32::GetSize()
{
   return sizeof( int32_t );
}

int32_t SerializedInt32::IntValue()
{
   return m_Value;
}

void SerializedInt32::CopyFrom( SerializedField& toCopy )
{
   m_Value = toCopy.IntValue();
}