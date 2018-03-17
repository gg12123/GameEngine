#pragma once
#include <list>
#include <unordered_map>
#include "TransformUpdater.h"
#include "GeometryRenderer.h"
#include "Physics.h"
#include "IInput.h"

class GameObject;
class SceneLoader;
class Editor;

class World
{
public:

   ~World();
   World();

   void Init( IWindowConfiguration& windowConfig, SceneLoader& sceneLoader, IInput& input );

   void DestroyHierarchy( GameObject& root ) const;
   void ClearAll();

   // When the world is awoken, all GOs in scene are instantiated and serialized fields good to go.
   // The vector of game objects is required becasue references between transforms and game objects is not
   // yet setup so the hierarchy cannot be iterated by the EnumerableHierarchy object.
   void Awake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects );
   void EditAwake( Editor& editor, GameObject& rootGameObject, std::vector<GameObject*>& gameObjects );

   // call start on GOs to be started at the beginning of each update
   void Update();
   void FixedUpdate();
   void EditUpdate();

   void RegisterForStart( GameObject &toRegister );
   std::list<GameObject*>::iterator RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject );
   void UnRegisterToUpdateFunction( EUpdaterFunction updateFunction, std::list<GameObject*>::iterator it );

   Transform& GetRootTransform();
   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();
   AssetLoader& GetAssetLoader();
   SceneLoader& GetSceneLoader();
   IInput& GetInput();
   Physics& GetPhysics();

private:

   void StartGameObjects();

   std::list<GameObject*> m_UpdatableGameObjects[ NUMBER_OF_UPDATE_FUNCTIONS ];
   std::list<GameObject*> m_GameObjectsToBeStarted;

   TransformUpdater m_TransformUpdater;
   GeometryRenderer m_GeometryRenderer;
   AssetLoader m_AssetLoader;
   Physics m_Physics;
   SceneLoader* m_SceneLoader;
   Transform *m_Root;
   IInput *m_Input;
};
