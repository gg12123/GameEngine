#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include "RenderingSlot.h"
#include "MeshStorage.h"
#include "ShaderProgramStorage.h"
#include "Camera.h"

typedef std::unordered_map<std::string, RenderingSlot*> MeshNameToRenderSlot;
typedef std::unordered_map<std::string, MeshNameToRenderSlot*> MeshNameToShaderNameToRenderSlot;

class GeometryRenderer
{
public:

   GeometryRenderer();

   // must be called before components are awake
   void Awake();

   void Render();

   std::list<MeshRenderer*>::iterator Register(MeshRenderer& const toReg);
   void UnRegister( MeshRenderer& toUnReg,  std::list<MeshRenderer*>::iterator toUnRegIter );

   void SetCamera( Camera& const cam );

private:

   GLuint m_Vao;

   // [shader name][mesh name]
   MeshNameToShaderNameToRenderSlot m_RenderingSlots;
   MeshStorage m_MeshStore;
   ShaderProgramStorage m_ProgramStore;

   Camera *m_Camera;
};
