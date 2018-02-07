#pragma once
#include "World.h"
#include "Functions.h"
#include "GameObject.h"

class Component
{
public:

   void Awake( World &world, GameObject &gameObject );

   virtual void Start();

protected:

   virtual void Awake();

   World& GetWorld();
   GameObject& GetGameObject();

   void RegisterUpdaterFunction( EUpdaterFunction eFunction, UpdaterFunctionPtr functionPtr );

private:

   World *m_World;
   GameObject *m_GameObject;
};