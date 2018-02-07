#include "Component.h"

void Component::Awake( World &world, GameObject &gameObject )
{
   m_World = &world;
   m_GameObject = &gameObject;

   Awake();
}

World& Component::GetWorld()
{
   return *m_World;
}

GameObject& Component::GetGameObject()
{
   return *m_GameObject;
}

void Component::RegisterUpdaterFunction( EUpdaterFunction eFunction, UpdaterFunctionPtr functionPtr )
{
   m_GameObject->RegisterUpdaterFunction( eFunction, functionPtr );
}

void Component::Start()
{
}

void Component::Awake()
{
}