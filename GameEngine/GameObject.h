#pragma once
#include <unordered_map>
#include <vector>
#include "Functions.h"
#include "World.h"
#include "Component.h"
#include "Transform.h"
#include "SerializedFields.h"

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

   void DeSerialize( std::ifstream& stream );
   void Serialize( std::ofstream& stream );

private:

   // do this inside DeSerialize()
   void CacheTransform();

   std::unordered_map<EUpdaterFunction, std::vector<UpdaterFunctionPtr>> m_UpdaterFunctions;
   std::vector<Component*> m_Components;

   World *m_World;
   Transform *m_Transform;
};
