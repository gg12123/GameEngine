#pragma once
#include <list>
#include "GameObject.h"
#include "TransformUpdater.h"

class World
{
public:

   // When the world is created, all GOs in scene are instantiated and serialized fields good to go
   World( GameObject& rootGameObject, std::list<GameObject*>& gameObjects );

   void Awake();

   // call start on GOs to be started at the beginning of each update
   void Update();
   void FixedUpdate();
   void EditUpdate();

   // for when a new GO gets instantiated
   void AddNewGameObject( GameObject &gameObject );

   void RegisterForStart( GameObject &toRegister );
   void RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject );

   Transform& GetRootTransform();
   TransformUpdater& GetTransformUpdater();

private:

   void UpdateGameObjects( EUpdaterFunction updateFunction );
   void StartGameObjects();

   std::map<EUpdaterFunction, std::list<GameObject*>> m_UpdatableGameObjects;

   std::list<GameObject*>* m_GameObjects;
   std::list<GameObject*> m_GameObjectsToBeStarted;

   TransformUpdater m_TransformUpdater;
   Transform *m_Root;
};
