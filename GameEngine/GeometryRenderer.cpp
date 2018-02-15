#include "GeometryRenderer.h"
#include "GL/gl3w.h"
#include "Debug.h"
#include "Vertex.h"
#include "ShaderLocations.h"

GeometryRenderer::GeometryRenderer()
{
   m_Camera = nullptr;
}

// maybe pass a config object
void GeometryRenderer::Awake()
{
   glGenVertexArrays( 1,
                      &m_Vao );

   glBindVertexArray( m_Vao );

   // Position
   glVertexAttribBinding( POS_VERT_ATTRIBUTE,
                          0 );
   glVertexAttribFormat( POS_VERT_ATTRIBUTE,
                         4,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof( Vertex, Position ) );
   glEnableVertexAttribArray( POS_VERT_ATTRIBUTE );

   // Normal
   glVertexAttribBinding( NORMAL_VERT_ATTRIBUTE,
                          0 );
   glVertexAttribFormat( NORMAL_VERT_ATTRIBUTE,
                         4,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof( Vertex, Normal ) );
   glEnableVertexAttribArray( NORMAL_VERT_ATTRIBUTE );

   // Tex cords
   glVertexAttribBinding( TC_VERT_ATTRIBUTE,
                          0 );
   glVertexAttribFormat( TC_VERT_ATTRIBUTE,
                         2,
                         GL_FLOAT,
                         GL_FALSE,
                         offsetof( Vertex, TexCords ) );
   glEnableVertexAttribArray( TC_VERT_ATTRIBUTE );
}

std::list<MeshRenderer*>::iterator GeometryRenderer::Register( MeshRenderer& const toReg )
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

void GeometryRenderer::SetCamera( Camera& const cam )
{
   if (m_Camera)
   {
      Debug::Instance().LogError( "Camera set twice" );
   }

   m_Camera = &cam;
}

void GeometryRenderer::Render()
{
   for (MeshNameToShaderNameToRenderSlot::iterator it = m_RenderingSlots.begin; it != m_RenderingSlots.end; it++)
   {
      glUseProgram( m_ProgramStore.GetProgram( it->first ) );

      // camera pos and world to view to proj matrix uniforms
      m_Camera->ApplyUniformsToShader();

      // set light,  (maybe the camera can do the matrix)

      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin; it2 != meshNameToSlot.end; it2++)
      {
         Mesh& m = m_MeshStore.GetMesh( it2->first );

         glBindVertexBuffer( 0,
                             m.GetVertexBuffer(),
                             0,
                             sizeof(Vertex) ); // TODO: change this zero to stride

         glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m.GetIndicesBuffer() );

         it2->second->Render( m.GetNumIndices() ); // the number of vertices to draw is equal to the number of indices
      }
   }
}