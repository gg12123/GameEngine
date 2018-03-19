#pragma once
#include "Component.h"

class EngineComponent : public Component
{
protected:
   void SetWorld( World& world ) override;

   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();
   Transform& GetRootTransform();
   AssetLoader& GetAssetLoader();
   Physics& GetPhysics();

private:
   World *m_World;
};
