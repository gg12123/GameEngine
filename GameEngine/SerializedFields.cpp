#include "SerializedFields.h"
#include "Debug.h"
#include "ImGUI/imgui.h"

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

// ####################### VECTOR3 ########################### 

void SerializedVector3::OnGUI( std::string name )
{
   ImGui::Text( name.c_str() );

   ImGui::InputFloat( "X", &m_Value[ 0 ] );
   ImGui::InputFloat( "Y", &m_Value[ 1 ] );
   ImGui::InputFloat( "Z", &m_Value[ 2 ] );
}

vmath::vec3 SerializedVector3::Value()
{
   return m_Value;
}

void SerializedVector3::SetValue( vmath::vec3 value )
{
   m_Value = value;
}

void SerializedVector3::LocalSerialize( std::ofstream& stream )
{
   SerializedFloat f;

   for (int i = 0; i < 3; i++)
   {
      f.SetValue( m_Value[ i ] );
      f.Serialize( stream );
   }
}

void SerializedVector3::LocalDeSerialize( std::ifstream& stream )
{
   SerializedFloat f;

   for (int i = 0; i < 3; i++)
   {
      f.DeSerialize( stream );
      m_Value[ i ] = f.Value();
   }
}

int32_t SerializedVector3::GetSize()
{
   return 3 * sizeof( float );
}

// ####################### ROTATION ########################### 

void SerializedRotation::OnGUI( std::string name )
{
   ImGui::Text( name.c_str() );
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

// ####################### FLOAT ########################### 

void SerializedFloat::OnGUI( std::string name )
{
   ImGui::InputFloat( name.c_str(), &m_Value );
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

// ####################### STRING ########################### 

void SerializedString::OnGUI( std::string name )
{
   char buff[ 50 ];
   ImGui::InputText( name.c_str(), buff, 50 );

   for (char* p = buff; p; p++)
   {
      m_Value.append( 1, *p );
   }
}

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

// ####################### INT32 ########################### 

void SerializedInt32::OnGUI( std::string name )
{
   ImGui::InputInt( name.c_str(), &m_Value );
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