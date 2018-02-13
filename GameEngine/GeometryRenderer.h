#pragma once
#include <map>
#include <string>
#include <list>
#include "RenderingSlot.h"
#include "MeshStorage.h"

typedef std::map<std::string, RenderingSlot*> MeshNameToRenderSlot;
typedef std::map<std::string, MeshNameToRenderSlot*> MeshNameToShaderNameToRenderSlot;

class GeometryRenderer
{
public:

   // must be called before components are awake
   void Awake();

   void Render();

   std::list<MeshRenderer*>::iterator Register(MeshRenderer& toReg);
   void UnRegister( std::list<MeshRenderer*>::iterator toUnReg );

private:

   GLuint m_Vao;

   // [shader name][mesh name]
   MeshNameToShaderNameToRenderSlot m_RenderingSlots;
   MeshStorage m_MeshStore;
};
