#pragma once
#include "GameObject.h"

class World
{
public:

   // When the world is created, all GOs in scene are instantiated and serialized fields good to go
   World( std::list<GameObject*>* gameObjects );

   void Awake();

   // call start on GOs to be started at the beginning of each update
   void Update();

   // for when a new GO gets instantiated
   void AddNewGameObject( GameObject *gameObject );

private:

   std::map<EUpdaterFunction, std::list<GameObject*>> m_UpdatableGameObjects;

   std::list<GameObject*>* m_GameObjects;
   std::list<GameObject*> m_GameObjectsToBeStarted();
};
