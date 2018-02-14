#include "GeometryRenderer.h"
#include "GL/gl3w.h"
#include "Debug.h"

// maybe pass a config object
void GeometryRenderer::Awake( std::string shaderLocation, std::string meshLocation )
{
   glGenVertexArrays( 1,
                      &m_Vao );

   glBindVertexArray( m_Vao );

   // setup vertex attributes

   m_ProgramStore.InitPathToShaders( shaderLocation );
}

std::list<MeshRenderer*>::iterator GeometryRenderer::Register( MeshRenderer& toReg )
{
   // load the mesh
   m_MeshStore.LoadMeshIfNotAlreadyLoaded( toReg.GetMeshName() );

   // load the shader
   m_ProgramStore.CompileIfNotAlready( toReg.GetShaderName() );

   // if this is a new shader name
   if (m_RenderingSlots.count( toReg.GetShaderName() ) == 0)
   {
      m_RenderingSlots[ toReg.GetShaderName() ] = new std::unordered_map<std::string, RenderingSlot*>();
   }

   MeshNameToRenderSlot &meshNameToSlot = *(m_RenderingSlots[ toReg.GetShaderName() ]);

   // if this is a new shader mesh combo
   if (meshNameToSlot.count( toReg.GetMeshName() ) == 0)
   {
      meshNameToSlot[ toReg.GetMeshName() ] = new RenderingSlot();
   }

   return meshNameToSlot[ toReg.GetMeshName() ]->Add( toReg );
}

void GeometryRenderer::UnRegister( MeshRenderer& toUnReg, std::list<MeshRenderer*>::iterator toUnRegIter )
{
   if (m_RenderingSlots.count( toUnReg.GetShaderName() ) == 0)
   {
      Debug::Instance().LogError( "Mesh renderer shader not registerd" );
   }
   else
   {
      MeshNameToRenderSlot &meshNameToSlot = *(m_RenderingSlots[ toUnReg.GetShaderName() ]);

      if (meshNameToSlot.count( toUnReg.GetMeshName() ) == 0)
      {
         Debug::Instance().LogError( "Mesh renderer mesh not registerd" );
      }
      else
      {
         meshNameToSlot[ toUnReg.GetMeshName() ]->Remove( toUnRegIter );
      }
   }
}

void GeometryRenderer::Render()
{
   for (MeshNameToShaderNameToRenderSlot::iterator it = m_RenderingSlots.begin; it != m_RenderingSlots.end; it++)
   {
      glUseProgram( m_ProgramStore.GetProgram( it->first ) );

      // set light and camera pos uniforms

      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin; it2 != meshNameToSlot.end; it2++)
      {
         Mesh& m = m_MeshStore.GetMesh( it2->first );

         glBindVertexBuffer( 0,
                             m.GetVertexBuffer(),
                             0,
                             0 ); // TODO: change this zero to stride

         glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m.GetIndicesBuffer() );

         it2->second->Render();
      }
   }
}