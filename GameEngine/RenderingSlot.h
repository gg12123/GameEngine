#pragma once
#include <list>
#include "MeshRenderer.h"

class RenderingSlot
{
public:

   RenderingSlot( Mesh& const mesh, GLuint shader );

   std::list<MeshRenderer*>::iterator Add( MeshRenderer& const meshRenderer );
   void Remove( const std::list<MeshRenderer*>::iterator toRemove );
   void Render();

   void UseProgram();
   void BindMesh();

private:

   std::list<MeshRenderer*> m_Renderers;
   Mesh* m_Mesh;
   GLuint m_Shader;
};
