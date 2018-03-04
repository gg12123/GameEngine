#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include "RenderingSlot.h"
#include "WindowConfiguration.h"
#include "AssetLoader.h"

class Light;
class Camera;

typedef std::unordered_map<std::string, RenderingSlot*> MeshNameToRenderSlot;
typedef std::unordered_map<std::string, MeshNameToRenderSlot*> ShaderNameToMeshNameToRenderSlot;

class GeometryRenderer
{
public:

   ~GeometryRenderer();
   GeometryRenderer();

   // must be called before components are awake
   void Awake( IWindowConfiguration& windowConfig, AssetLoader& assetLoader );

   void Render();

   std::list<MeshRenderer*>::iterator Register(MeshRenderer& toReg);
   void UnRegister( MeshRenderer& toUnReg,  std::list<MeshRenderer*>::iterator toUnRegIter );

   IWindowConfiguration& SetCamera( Camera& cam );
   void SetLight( Light& light );

   void OnDestroy();

private:

   GLuint m_Vao;

   // [shader name][mesh name]
   ShaderNameToMeshNameToRenderSlot m_RenderingSlots;

   Camera *m_Camera;
   IWindowConfiguration *m_WindowConfig;
   AssetLoader *m_AssetLoader;
   Light *m_Light;
};
