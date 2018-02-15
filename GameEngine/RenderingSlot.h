#pragma once
#include <list>
#include "MeshRenderer.h"

class RenderingSlot
{
public:

   std::list<MeshRenderer*>::iterator Add( MeshRenderer& const meshRenderer );
   void Remove( const std::list<MeshRenderer*>::iterator toRemove );
   void Render(const int count);

private:

   std::list<MeshRenderer*> m_Renderers;
};
