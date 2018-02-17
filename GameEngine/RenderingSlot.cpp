#include "RenderingSlot.h"
#include "Mesh.h"
#include "Vertex.h"

std::list<MeshRenderer*>::iterator RenderingSlot::Add( MeshRenderer& const meshRenderer )
{
   m_Renderers.push_back( &meshRenderer );
   return m_Renderers.back;
}

void RenderingSlot::Remove(const std::list<MeshRenderer*>::iterator toRemove )
{
   m_Renderers.erase( toRemove );
}

void RenderingSlot::Render()
{
   for (std::list<MeshRenderer*>::iterator it = m_Renderers.begin; it != m_Renderers.end; it++)
   {
      (*it)->Render(m_Mesh->GetNumIndices());
   }
}

void RenderingSlot::UseProgram()
{
   glUseProgram( m_Shader );
}

void RenderingSlot::BindMesh()
{
   glBindVertexBuffer( 0,
                       m_Mesh->GetVertexBuffer(),
                       0,
                       sizeof( Vertex ) );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Mesh->GetIndicesBuffer() );
}