#include "World.h"
#include "Utils.h"

World::World()
{
   m_Root = nullptr;
}

void World::Awake( GameObject& rootGameObject, WindowConfiguration& windowConfig )
{
   m_Root = &rootGameObject.GetTransfrom();

   m_GeometryRenderer.Awake( windowConfig, m_AssetLoader );

   EnumerableHierarchy enumerator( m_Root->GetGameObject() );
   GameObject *next = enumerator.Next();

   while (next != nullptr)
   {
      next->AwakeComponents( *this );
      next = enumerator.Next();
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