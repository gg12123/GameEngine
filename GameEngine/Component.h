#pragma once
#include <unordered_map>
#include <string>
#include "Functions.h"
#include "SerializedFields.h"
#include "PrefabField.h"
#include "MeshField.h"
#include "ISearializedFieldOwner.h"

class GameObject;
class World;
class TransformUpdater;
class Transform;
class GeometryRenderer;

class Component : public ISerializedFieldOwner
{
public:

   virtual ~Component();

   void Awake( World &world, GameObject &gameObject );

   virtual void Start();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void DeSerialize( std::ifstream& stream ) override;
   void Serialize( std::ofstream& stream ) override;
   std::string GetName() override;
   void OnNewSerializedFields() override;

   GameObject& GetGameObject();

   virtual int32_t GetType() = 0;

   virtual void Update();
   virtual void FixedUpdate();
   virtual void EditUpdate();

protected:

   virtual void Awake();

   void RegisterForUpdate( const EUpdaterFunction eFunction );

   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();
   Transform& GetRootTransform();

   GameObject& InstantiatePrefab( const PrefabField& prefab );
   std::string InstantiateMesh( const MeshField& mesh );

private:

   World *m_World;
   GameObject *m_GameObject;
};