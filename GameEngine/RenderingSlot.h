#pragma once
#include <list>
#include "MeshRenderer.h"

class RenderingSlot
{
public:

   std::list<MeshRenderer*>::iterator Add( MeshRenderer& meshRenderer );
   void Remove( std::list<MeshRenderer*>::iterator toRemove );
   void Render();

private:

   std::list<MeshRenderer*> m_Renderers;
};
