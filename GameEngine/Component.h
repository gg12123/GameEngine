#pragma once
#include <unordered_map>
#include <string>
#include "Functions.h"
#include "SerializedFields.h"
#include "PrefabField.h"
#include "MeshField.h"

class GameObject;
class World;
class TransformUpdater;
class Transform;
class GeometryRenderer;

class Component
{
public:

   virtual ~Component();

   void Awake( World &world, GameObject &gameObject );

   virtual void Start();

   virtual void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields );
   void DeSerialize( std::ifstream& stream );
   void Serialize( std::ofstream& stream );

   GameObject& GetGameObject();

   virtual int32_t GetType() = 0;
   virtual std::string GetName() = 0;

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