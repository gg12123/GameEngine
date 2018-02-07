#include "World.h"

World::World( GameObject& rootGameObject, std::list<GameObject*>& gameObjects )
{
   m_GameObjects = &gameObjects;
}

void World::Awake()
{
   for (std::list<GameObject*>::iterator it = m_GameObjects->begin; it != m_GameObjects->end; it++)
   {
      (*it)->AwakeComponents( *this );
   }
}

// call start on GOs to be started at the beginning of each update
void World::Update()
{
   StartGameObjects();
   UpdateGameObjects( eUpdateFunction );

   // update dirty transforms

   // render
}

void World::FixedUpdate()
{
   StartGameObjects();
   UpdateGameObjects( eFixedUpdateFunction );

   // do some physics stuff
}

// for when a new GO gets instantiated
void World::AddNewGameObject( GameObject &gameObject )
{
   m_GameObjects->push_back( &gameObject );
}

void World::RegisterForStart( GameObject &toRegister )
{
   m_GameObjectsToBeStarted.push_back( &toRegister );
}

void World::RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject )
{
   m_UpdatableGameObjects[ updateFunction ].push_back( &gameObject );
}

void World::UpdateGameObjects( EUpdaterFunction updateFunction )
{
   std::list<GameObject*>* toUpdate = &m_UpdatableGameObjects[ updateFunction ];

   for (std::list<GameObject*>::iterator it = toUpdate->begin; it != toUpdate->end; it++)
   {
      (*it)->UpdateComponents( updateFunction );
   }
}

void World::StartGameObjects()
{
   for (std::list<GameObject*>::iterator it = m_GameObjectsToBeStarted.begin; it != m_GameObjectsToBeStarted.end; it++)
   {
      (*it)->StartComponents();
   }

   m_GameObjectsToBeStarted.clear();
}