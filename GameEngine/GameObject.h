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

   void AwakeComponents( World &world );
   void StartComponents();
   void UpdateComponents( EUpdaterFunction updateFunction );

   void AddComponent( Component &component );
   void RegisterUpdaterFunction( EUpdaterFunction updateFunction, UpdaterFunctionPtr updaterPtr );

   void Update();
   void FixedUpdate();

   Transform& GetTransfrom();

private:

   std::map<EUpdaterFunction, std::list<UpdaterFunctionPtr>> m_UpdaterFunctions;
   std::list<Component*> m_Components;

   World *m_World;
   Transform m_Transform;
};
