#include "World.h"
#include "Utils.h"
#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "SceneLoader.h"

World::~World()
{
   ClearAll();
}

void World::DestroyHierarchy( GameObject& root ) const
{
   EnumerableHierarchy enumerator( root );
   std::vector<GameObject*> toDestroy;

   InvokeEvent( eOnHierarchyBeingDestroyed, root );

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
   m_SceneLoader = nullptr;
   m_Input = nullptr;
}

void World::ClearAll()
{
   if (m_Root)
   {
      m_GameObjectsToBeStarted.clear();
      DestroyHierarchy( m_Root->GetGameObject() );

      for (int i = 0; i < NUMBER_OF_UPDATE_FUNCTIONS; i++)
      {
         if (m_UpdatableGameObjects[ i ].size() > 0)
         {
            Debug::Instance().LogError( "Updateable game objects not cleared in clear all" );
         }
      }
      m_Root = nullptr;
   }
}

void World::Init( IWindowConfiguration& windowConfig, SceneLoader& loader, IInput& input )
{
   m_GeometryRenderer.Init( windowConfig, m_AssetLoader );
   m_SceneLoader = &loader;
   m_Input = &input;
}

void World::Awake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects )
{
   m_Root = &rootGameObject.GetTransform();

   for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
   {
      (*it)->AwakeComponents( *this );
   }
}

void World::EditAwake( Editor& editor, GameObject& rootGameObject, std::vector<GameObject*>& gameObjects )
{
   m_Root = &rootGameObject.GetTransform();

   for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
   {
      (*it)->EditAwakeComponents( editor );
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
   m_SceneLoader->Update();
}

void World::EditUpdate()
{
   // no need to call start in edit mode
   
   auto l = &m_UpdatableGameObjects[ eUpdateInEditMode ];
   for (auto it = l->begin(); it != l->end(); it++)
   {
      (*it)->EditUpdate();
   }

   m_TransformUpdater.UpdateTransforms();
   m_GeometryRenderer.Render();
   m_SceneLoader->Update();
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
   m_UpdatableGameObjects[ updateFunction ].push_front( &gameObject );
   return m_UpdatableGameObjects[ updateFunction ].begin();
}

void World::UnRegisterToUpdateFunction( EUpdaterFunction updateFunction, std::list<GameObject*>::iterator it )
{
   m_UpdatableGameObjects[ updateFunction ].erase( it );
}

void World::RegisterToEvent( const EWorldEvent eventID, EventHandler& handler )
{
   m_EventManager.Register( eventID, handler );
}

void World::InvokeEvent( const EWorldEvent eventID, GameObject& param ) const
{
   m_EventManager.Invoke<GameObject&>( eventID, param );
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

SceneLoader& World::GetSceneLoader()
{
   return *m_SceneLoader;
}

IInput& World::GetInput()
{
   return *m_Input;
}

Physics& World::GetPhysics()
{
   return m_Physics;
}