#pragma once
#include <list>
#include "MeshRenderer.h"
#include "Mesh.h"

class RenderingSlot
{
public:

   ~RenderingSlot();
   RenderingSlot( Mesh& mesh, GLuint shader );

   std::list<MeshRenderer*>::iterator Add( MeshRenderer& meshRenderer );
   void Remove( const std::list<MeshRenderer*>::iterator toRemove );
   void Render();

   void UseProgram();
   void BindMesh();

   void OnDestroy();

private:

   std::list<MeshRenderer*> m_Renderers;
   Mesh* m_Mesh;
   GLuint m_Shader;
};
