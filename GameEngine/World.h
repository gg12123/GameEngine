#pragma once
#include <list>
#include <unordered_map>
#include "TransformUpdater.h"
#include "GeometryRenderer.h"

class GameObject;

class World
{
public:

   World();

   // When the world is awoken, all GOs in scene are instantiated and serialized fields good to go
   void Awake( GameObject& rootGameObject, IWindowConfiguration& windowConfig );

   // call start on GOs to be started at the beginning of each update
   void Update();
   void FixedUpdate();
   void EditUpdate();

   void RegisterForStart( GameObject &toRegister );
   void RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject );

   Transform& GetRootTransform();
   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();

private:

   void UpdateGameObjects( EUpdaterFunction updateFunction );
   void StartGameObjects();

   std::unordered_map<EUpdaterFunction, std::list<GameObject*>*> m_UpdatableGameObjects;
   std::list<GameObject*> m_GameObjectsToBeStarted;

   TransformUpdater m_TransformUpdater;
   GeometryRenderer m_GeometryRenderer;
   AssetLoader m_AssetLoader;
   Transform *m_Root;
};
