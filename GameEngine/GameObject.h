#pragma once
#include <map>
#include <list>
#include "Functions.h"
#include "World.h"
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:

   GameObject();

   void AwakeComponents( World *world );
   void StartComponents();
   void AddComponent( Component *component );
   void RegisterUpdaterFunction( EUpdaterFunction eUpdater, UpdaterFunctionPtr updaterPtr );

   void Update();
   void FixedUpdate();

   Transform *GetTransfrom();

private:

   void CallUpdaterFunctions( EUpdaterFunction eUpdater );

   std::map<EUpdaterFunction, std::list<UpdaterFunctionPtr>> m_UpdaterFunctions;
   std::list<Component*> m_Components;

   World *m_World;
   Transform m_Transform;
};
