#pragma once
#include <list>
#include <unordered_map>
#include "TransformUpdater.h"
#include "GeometryRenderer.h"

class GameObject;

class World
{
public:

   ~World();
   World();

   void OnDestroy();

   // When the world is awoken, all GOs in scene are instantiated and serialized fields good to go.
   // The vector of game objects is required becasue references between transforms and game objects is not
   // yet setup so the hierarchy cannot be iterated by the EnumerableHierarchy object.
   void Awake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects, IWindowConfiguration& windowConfig );

   // call start on GOs to be started at the beginning of each update
   void Update();
   void FixedUpdate();
   void EditUpdate();

   void RegisterForStart( GameObject &toRegister );
   void RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject );

   Transform& GetRootTransform();
   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();
   AssetLoader& GetAssetLoader();

private:

   void StartGameObjects();

   std::list<GameObject*> m_UpdatableGameObjects[ NUMBER_OF_UPDATE_FUNCTIONS ];
   std::list<GameObject*> m_GameObjectsToBeStarted;

   TransformUpdater m_TransformUpdater;
   GeometryRenderer m_GeometryRenderer;
   AssetLoader m_AssetLoader;
   Transform *m_Root;
};
