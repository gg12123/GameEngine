#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include "RenderingSlot.h"
#include "MeshStorage.h"
#include "ShaderProgramStorage.h"

typedef std::unordered_map<std::string, RenderingSlot*> MeshNameToRenderSlot;
typedef std::unordered_map<std::string, MeshNameToRenderSlot*> MeshNameToShaderNameToRenderSlot;

class GeometryRenderer
{
public:

   // must be called before components are awake
   void Awake(std::string shaderLocation, std::string meshLocation);

   void Render();

   std::list<MeshRenderer*>::iterator Register(MeshRenderer& toReg);
   void UnRegister( MeshRenderer& toUnReg,  std::list<MeshRenderer*>::iterator toUnRegIter );

private:

   GLuint m_Vao;

   // [shader name][mesh name]
   MeshNameToShaderNameToRenderSlot m_RenderingSlots;
   MeshStorage m_MeshStore;
   ShaderProgramStorage m_ProgramStore;
};
