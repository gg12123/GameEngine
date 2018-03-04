#include "World.h"
#include "Utils.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"

World::~World()
{
   EnumerableHierarchy enumerator( m_Root->GetGameObject() );

   GameObject* next = enumerator.Next();

   while (next)
   {
      delete next;
      next = enumerator.Next();
   }

   m_Root = nullptr;
}

void World::OnDestroy()
{
   EnumerableHierarchy enumerator( m_Root->GetGameObject() );

   GameObject* next = enumerator.Next();

   while (next)
   {
      // call on destroy
      next = enumerator.Next();
   }

   for (int i = 0; i < NUMBER_OF_UPDATE_FUNCTIONS; i++)
   {
      m_UpdatableGameObjects[ i ].clear();
   }

   m_GeometryRenderer.OnDestroy();
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

void World::RegisterToUpdateFunction( EUpdaterFunction updateFunction, GameObject& gameObject )
{
   std::list<GameObject*>* l = &m_UpdatableGameObjects[ updateFunction ];

   auto it = std::find( l->begin(), l->end(), &gameObject );

   if (it != l->end())
   {
      Debug::Instance().LogError( "Game object registerd twice for update" );
   }

   l->push_back( &gameObject );
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