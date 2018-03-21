#pragma once
#include "Component.h"

class TransformUpdater;
class GeometryRenderer;
class AssetLoader;
class Physics;

class EngineComponent : public Component
{
public:
   EngineComponent();

protected:
   void SetWorld( World& world ) override final;

   TransformUpdater& GetTransformUpdater();
   GeometryRenderer& GetGeometryRenderer();
   Transform& GetRootTransform();
   AssetLoader& GetAssetLoader();
   Physics& GetPhysics();

private:
   World *m_World;
};
