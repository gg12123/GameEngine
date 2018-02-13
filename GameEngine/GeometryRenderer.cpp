#include "GeometryRenderer.h"
#include "GL/gl3w.h"

void GeometryRenderer::Awake()
{
   glGenVertexArrays( 1,
                      &m_Vao );

   glBindVertexArray( m_Vao );

   // setup vertex attributes
}

std::list<MeshRenderer*>::iterator GeometryRenderer::Register( MeshRenderer& toReg )
{
   // load the mesh
   m_MeshStore.LoadMeshIfNotAlreadyLoaded( toReg.GetMeshName() );

   // load the shader

   // if this is a new shader name
   if (m_RenderingSlots.count( toReg.GetShaderName() ) == 0)
   {
      m_RenderingSlots[ toReg.GetShaderName() ] = new std::map<std::string, RenderingSlot*>();
   }

   MeshNameToRenderSlot &meshNameToSlot = *(m_RenderingSlots[ toReg.GetShaderName() ]);

   // if this is a new shader mesh combo
   if (meshNameToSlot.count( toReg.GetMeshName() ) == 0)
   {
      meshNameToSlot[ toReg.GetMeshName() ] = new RenderingSlot();
   }

   return meshNameToSlot[ toReg.GetMeshName() ]->Add( toReg );
}

void GeometryRenderer::Render()
{
   for (MeshNameToShaderNameToRenderSlot::iterator it = m_RenderingSlots.begin; it != m_RenderingSlots.end; it++)
   {
      // call glUseProgram with it->first

      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin; it2 != meshNameToSlot.end; it2++)
      {
         glBindVertexBuffer( 0,
                             m_MeshStore.GetMesh( it2->first ).GetVertexBuffer(),
                             0,
                             0 ); // TODO: change this zero to stride

         it2->second->Render();
      }
   }
}