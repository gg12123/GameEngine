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

protected:

   virtual void Awake();

   GameObject& GetGameObject();

   void RegisterUpdaterFunction( EUpdaterFunction eFunction, UpdaterFunctionPtr functionPtr );

   TransformUpdater& GetTransformUpdater();
   Transform& GetRootTransform();

private:

   World *m_World;
   GameObject *m_GameObject;
};