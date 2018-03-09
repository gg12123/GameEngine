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

void SerializedVector3::InitForGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   m_ElementNames[ 0 ] = fieldName + " X";
   m_ElementNames[ 1 ] = fieldName + " Y";
   m_ElementNames[ 2 ] = fieldName + " Z";
}

void SerializedVector3::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::Text( fieldName.c_str() );

   if (ImGui::InputFloat( m_ElementNames[ 0 ].c_str(), &m_Value[ 0 ] ))
   {
      owner.OnNewSerializedFields();
   }

   if (ImGui::InputFloat( m_ElementNames[ 1 ].c_str(), &m_Value[ 1 ] ))
   {
      owner.OnNewSerializedFields();
   }

   if (ImGui::InputFloat( m_ElementNames[ 2 ].c_str(), &m_Value[ 2 ] ))
   {
      owner.OnNewSerializedFields();
   }
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

void SerializedRotation::InitForGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   m_Euler = vmath::matrixToEuler( m_Value );
}

void SerializedRotation::OnGUI( std::string fieldName, ISerializedFieldOwner& owner )
{
   ImGui::Text( fieldName.c_str() );

   bool inputDone = false;

   if (ImGui::InputFloat( "Y (yaw)", &m_Euler[ 1 ] ))
   {
      inputDone = true;
   }

   if (ImGui::InputFloat( "X (pitch)", &m_Euler[ 0 ] ))
   {
      inputDone = true;
   }

   if (ImGui::InputFloat( "Z (roll)", &m_Euler[ 2 ] ))
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