#include "EngineComponent.h"
#include "World.h"

EngineComponent::EngineComponent()
{
   m_World = nullptr;
}

void EngineComponent::SetWorld( World& world )
{
   m_World = &world;
}

TransformUpdater& EngineComponent::GetTransformUpdater()
{
   return m_World->GetTransformUpdater();
}

Transform& EngineComponent::GetRootTransform()
{
   return m_World->GetRootTransform();
}

Physics& EngineComponent::GetPhysics()
{
   return m_World->GetPhysics();
}

GeometryRenderer& EngineComponent::GetGeometryRenderer()
{
   return m_World->GetGeometryRenderer();
}

AssetLoader& EngineComponent::GetAssetLoader()
{
   return m_World->GetAssetLoader();
}