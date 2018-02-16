#include "Component.h"

void Component::Awake( World &world, GameObject &gameObject )
{
   m_World = &world;
   m_GameObject = &gameObject;

   Awake();
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

TransformUpdater& Component::GetTransformUpdater()
{
   return m_World->GetTransformUpdater();
}

Transform& Component::GetRootTransform()
{
   return m_World->GetRootTransform();
}

GeometryRenderer& Component::GetGeometryRenderer()
{
   return m_World->GetGeometryRenderer();
}