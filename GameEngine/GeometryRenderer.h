#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include "RenderingSlot.h"
#include "Camera.h"
#include "WindowConfiguration.h"
#include "AssetLoader.h"
#include "Light.h"

typedef std::unordered_map<std::string, RenderingSlot*> MeshNameToRenderSlot;
typedef std::unordered_map<std::string, MeshNameToRenderSlot*> MeshNameToShaderNameToRenderSlot;

class GeometryRenderer
{
public:

   GeometryRenderer();

   // must be called before components are awake
   void Awake( IWindowConfiguration& const windowConfig, AssetLoader& const assetLoader );

   void Render();

   std::list<MeshRenderer*>::iterator Register(MeshRenderer& const toReg);
   void UnRegister( MeshRenderer& toUnReg,  std::list<MeshRenderer*>::iterator toUnRegIter );

   IWindowConfiguration& SetCamera( Camera& const cam );
   void SetLight( Light& const light );

private:

   GLuint m_Vao;

   // [shader name][mesh name]
   MeshNameToShaderNameToRenderSlot m_RenderingSlots;

   Camera *m_Camera;
   IWindowConfiguration *m_WindowConfig;
   AssetLoader *m_AssetLoader;
   Light *m_Light;
};
