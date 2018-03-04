#include "World.h"
#include "Utils.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"

World::~World()
{
   DestroyHierarchy( m_Root->GetGameObject() );

   for (int i = 0; i < NUMBER_OF_UPDATE_FUNCTIONS; i++)
   {
      if (m_UpdatableGameObjects[ i ].size() > 0)
      {
         Debug::Instance().LogError( "Updateable game objects not cleared on destroy " );
      }
   }
}

void World::DestroyHierarchy( GameObject& root ) const
{
   EnumerableHierarchy enumerator( root );
   std::vector<GameObject*> toDestroy;

   GameObject* next = enumerator.Next();

   while (next)
   {
      toDestroy.push_back( next );
      next->OnDestroy(); // this call will break transform parent child links, hence the need for the vector.
      next = enumerator.Next();
   }

   for (auto it = toDestroy.begin(); it != toDestroy.end(); it++)
   {
      delete *it;
   }
}

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
   
   auto l = &m_UpdatableGameObjects[ eUpdateFunction ];
   for (auto it = l->begin(); it != l->end(); it++)
   {
      (*it)->Update();
   }

   m_TransformUpdater.UpdateTransforms();
   m_GeometryRenderer.Render();
}

void World::EditUpdate()
{
   StartGameObjects(); // may not need to call start here
   
   auto l = &m_UpdatableGameObjects[ eUpdateInEditMode ];
   for (auto it = l->begin(); it != l->end(); it++)
   {
      (*it)->EditUpdate();
   }

   m_TransformUpdater.UpdateTransforms();
   m_GeometryRenderer.Render();
}

void World::FixedUpdate()
{
   StartGameObjects();
   
   auto l = &m_UpdatableGameObjects[ eFixedUpdateFunction ];
   for (auto it = l->begin(); it != l->end(); it++)
   {
      (*it)->FixedUpdate();
   }

   // do some physics stuff
}

void World::RegisterForStart( GameObject &toRegister )
{
   m_GameObjectsToBeStarted.push_back( &toRegister );
}

std::list<GameObject*>::iterator World::RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject )
{
   std::list<GameObject*>* l = &m_UpdatableGameObjects[ updateFunction ];

   auto it = std::find( l->begin(), l->end(), &gameObject );

   if (it != l->end())
   {
      Debug::Instance().LogError( "Game object registerd twice for update" );
   }

   l->push_front( &gameObject );

   return l->begin();
}

void World::UnRegisterToUpdateFunction( EUpdaterFunction updateFunction, std::list<GameObject*>::iterator it )
{
   m_UpdatableGameObjects[ updateFunction ].erase( it );
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

AssetLoader& World::GetAssetLoader()
{
   return m_AssetLoader;
}