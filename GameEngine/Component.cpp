#include "Component.h"
#include "GameObject.h"
#include "World.h"
#include "TransformUpdater.h"
#include "Transform.h"
#include "GeometryRenderer.h"

void Component::Awake( World &world, GameObject &gameObject )
{
   m_World = &world;
   m_GameObject = &gameObject;

   Awake();
}

GameObject& Component::GetGameObject()
{
   return *m_GameObject;
}

void Component::RegisterForUpdate( const EUpdaterFunction eFunction )
{
   m_GameObject->RegisterComponentForUpdate( eFunction, *this );
}

void Component::Start()
{
}

void Component::Awake()
{
}

TransformUpdater& Component::GetTransformUpdater()
{
   return m_World->GetTransformUpdater();
}

Transform& Component::GetRootTransform()
{
   return m_World->GetRootTransform();
}

GeometryRenderer& Component::GetGeometryRenderer()
{
   return m_World->GetGeometryRenderer();
}

void  Component::Serialize( std::ofstream& stream )
{
   SerializedInt32 number;
   SerializedString fieldName;
   std::unordered_map<std::string, SerializedField*> fields;

   GetSerializedFields( fields );

   number.SetValue( fields.size() );
   number.Serialize( stream );

   for (std::unordered_map<std::string, SerializedField*>::iterator it = fields.begin(); it != fields.end(); it++)
   {
      fieldName.SetValue( it->first );
      fieldName.Serialize( stream );

      it->second->SerializeWithSize( stream );
   }
}

void Component::DeSerialize( std::ifstream& stream )
{
   SerializedInt32 number;
   number.DeSerialize( stream );
   int32_t count = number.Value();

   std::unordered_map<std::string, SerializedField*> fields;
   std::unordered_map<std::string, SerializedField*>::iterator it;

   GetSerializedFields( fields );

   SerializedString fieldName;

   for (int i = 0; i < count; i++)
   {
      fieldName.DeSerialize( stream );

      it = fields.find( fieldName.Value() );

      if (it != fields.end())
      {
         it->second->DeSerializeWithSize( stream );
      }
      else
      {
         // seek the stream past the size of the unknown field
         number.DeSerialize( stream );
         stream.seekg( number.Value(), stream.cur );
      }
   }
}

void Component::GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields )
{
}

GameObject& Component::InstantiatePrefab( const PrefabField& prefab )
{
   return prefab.Instantiate( *m_World );
}

std::string Component::InstantiateMesh( const MeshField& mesh )
{
   return mesh.Instantiate( *m_World );
}

void Component::Update()
{
   throw std::exception( "Update called with no implementation" );
}

void Component::FixedUpdate()
{
   throw std::exception( "Fixed update called with no implementation" );
}

void Component::EditUpdate()
{
   throw std::exception( "Edit update called with no implementation" );
}