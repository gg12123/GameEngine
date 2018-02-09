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

   // called by the world if this object starts in the scene. If the object is instantiated, awake
   // must be called by the instantitor.
   void AwakeComponents( World &world );

   // called by the world before the next update
   void StartComponents();

   // called by the world
   void UpdateComponents( EUpdaterFunction updateFunction );

   void AddComponent( Component &component );
   void RegisterUpdaterFunction( EUpdaterFunction updateFunction, UpdaterFunctionPtr updaterPtr );

   Transform& GetTransfrom();

   void CacheTransform();

private:

   std::map<EUpdaterFunction, std::list<UpdaterFunctionPtr>> m_UpdaterFunctions;
   std::list<Component*> m_Components;

   World *m_World;
   Transform *m_Transform;
};
