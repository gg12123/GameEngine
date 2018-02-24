#include "World.h"
#include "Utils.h"
#include "GameObject.h"
#include "Transform.h"

World::World()
{
   m_Root = nullptr;
}

void World::Awake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects, IWindowConfiguration& windowConfig )
{
   m_Root = &rootGameObject.GetTransform();

   m_GeometryRenderer.Awake( windowConfig, m_AssetLoader );

   for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
   {
      (*it)->AwakeComponents( *this );
   }
}

// call start on GOs to be started at the beginning of each update
void World::Update()
{
   StartGameObjects();
   UpdateGameObjects( eUpdateFunction );

   m_TransformUpdater.UpdateTransforms();

   m_GeometryRenderer.Render();
}

void World::EditUpdate()
{
   StartGameObjects(); // may not need to call start here
   UpdateGameObjects( eUpdateInEditMode );

   m_TransformUpdater.UpdateTransforms();

   m_GeometryRenderer.Render();
}

void World::FixedUpdate()
{
   StartGameObjects();
   UpdateGameObjects( eFixedUpdateFunction );

   // do some physics stuff
}

void World::RegisterForStart( GameObject &toRegister )
{
   m_GameObjectsToBeStarted.push_back( &toRegister );
}

void World::RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject )
{
   std::unordered_map<EUpdaterFunction, std::list<GameObject*>*>::iterator it;
   std::list<GameObject*>* lst;

   it = m_UpdatableGameObjects.find( updateFunction );

   if (it == m_UpdatableGameObjects.end())
   {
      lst = new std::list<GameObject*>();
      m_UpdatableGameObjects[ updateFunction ] = lst;
   }
   else
   {
      lst = it->second;
   }

   lst->push_back( &gameObject );
}

void World::UpdateGameObjects( EUpdaterFunction updateFunction )
{
   std::unordered_map<EUpdaterFunction, std::list<GameObject*>*>::iterator it;
   it = m_UpdatableGameObjects.find( updateFunction );

   if (it != m_UpdatableGameObjects.end())
   {
      std::list<GameObject*>* toUpdate = it->second;

      for (std::list<GameObject*>::iterator it = toUpdate->begin(); it != toUpdate->end(); it++)
      {
         (*it)->UpdateComponents( updateFunction );
      }
   }
}

void World::StartGameObjects()
{
   for (std::list<GameObject*>::iterator it = m_GameObjectsToBeStarted.begin(); it != m_GameObjectsToBeStarted.end(); it++)
   {
      (*it)->StartComponents();
   }

   m_GameObjectsToBeStarted.clear();
}

Transform& World::GetRootTransform()
{
   return *m_Root;
}

TransformUpdater& World::GetTransformUpdater()
{
   return m_TransformUpdater;
}

GeometryRenderer& World::GetGeometryRenderer()
{
   return m_GeometryRenderer;
}