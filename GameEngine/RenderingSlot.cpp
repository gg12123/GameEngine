#include "RenderingSlot.h"

std::list<MeshRenderer*>::iterator RenderingSlot::Add( MeshRenderer& const meshRenderer )
{
   m_Renderers.push_back( &meshRenderer );
   return m_Renderers.back;
}

void RenderingSlot::Remove(const std::list<MeshRenderer*>::iterator toRemove )
{
   m_Renderers.erase( toRemove );
}

void RenderingSlot::Render(const int count)
{
   for (std::list<MeshRenderer*>::iterator it = m_Renderers.begin; it != m_Renderers.end; it++)
   {
      (*it)->Render(count);
   }
}