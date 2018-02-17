#pragma once
#include "World.h"
#include "Functions.h"
#include "GameObject.h"

class Component
{
public:

   void Awake( World &world, GameObject &gameObject );

   virtual void Start();
   virtual void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields );

   GameObject& GetGameObject();

protected:

   virtual void Awake();

   void RegisterUpdaterFunction( EUpdaterFunction eFunction, UpdaterFunctionPtr functionPtr );

   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();
   Transform& GetRootTransform();

private:

   World *m_World;
   GameObject *m_GameObject;
};