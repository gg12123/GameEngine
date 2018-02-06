#include "GameObject.h"

GameObject::GameObject()
{
   AddComponent( &m_Transform );
}

Transform* GameObject::GetTransfrom()
{
   return &m_Transform;
}

void GameObject::Update()
{
   CallUpdaterFunctions( eUpdateFunction );
}

void GameObject::FixedUpdate()
{
   CallUpdaterFunctions( eFixedUpdateFunction );
}

void GameObject::CallUpdaterFunctions( EUpdaterFunction eUpdater )
{
   for (std::list<UpdaterFunctionPtr>::iterator it = m_UpdaterFunctions[ eUpdater ].begin; it != m_UpdaterFunctions[ eUpdater ].end; it++)
   {
      // call it
   }
}

void GameObject::AwakeComponents( World *world )
{
   m_World = world;

   for (std::list<Component*>::iterator it = m_Components.begin; it != m_Components.end; it++)
   {
      (*it)->Awake( world, this );
   }
}

void GameObject::StartComponents()
{
   for (std::list<Component*>::iterator it = m_Components.begin; it != m_Components.end; it++)
   {
      (*it)->Start();
   }
}

void GameObject::AddComponent( Component *component )
{
   m_Components.push_back( component );
}

void GameObject::RegisterUpdaterFunction( EUpdaterFunction eUpdater, UpdaterFunctionPtr updaterPtr )
{
   if (m_UpdaterFunctions[ eUpdater ].size == 0)
   {
      // tell world to add this GO to the updater list
   }

   m_UpdaterFunctions[ eUpdater ].push_back( updaterPtr );
}