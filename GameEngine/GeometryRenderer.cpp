#include "GeometryRenderer.h"
#include "GL/gl3w.h"
#include "Debug.h"
#include "Vertex.h"
#include "ShaderLocations.h"
#include "Mesh.h"
#include "ShaderProgram.h"

GeometryRenderer::GeometryRenderer()
{
   m_Camera = nullptr;
   m_WindowConfig = nullptr;
   m_AssetLoader = nullptr;
   m_Light = nullptr;
}

// maybe pass a config object
void GeometryRenderer::Awake( WindowConfiguration& const windowConfig, AssetLoader& const assetLoader )
{
   m_WindowConfig = &windowConfig;
   m_AssetLoader = &assetLoader;

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
   // if this is a new shader name
   if (m_RenderingSlots.count( toReg.GetShaderName() ) == 0)
   {
      m_RenderingSlots[ toReg.GetShaderName() ] = new std::unordered_map<std::string, RenderingSlot*>();
   }

   MeshNameToRenderSlot &meshNameToSlot = *(m_RenderingSlots[ toReg.GetShaderName() ]);

   // if this is a new shader mesh combo
   if (meshNameToSlot.count( toReg.GetMeshName() ) == 0)
   {
      Mesh* mesh = m_AssetLoader->LoadIfNotAlreadyLoaded( toReg.GetMeshName(), Mesh::CreateInstance ).MeshValue();
      GLuint shader = m_AssetLoader->LoadIfNotAlreadyLoaded( toReg.GetShaderName(), ShaderProgram::CreateInstance ).ShaderValue();

      meshNameToSlot[ toReg.GetMeshName() ] = new RenderingSlot( *mesh, shader );
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

WindowConfiguration& GeometryRenderer::SetCamera( Camera& const cam )
{
   if (m_Camera)
   {
      Debug::Instance().LogError( "Camera set twice" );
   }

   m_Camera = &cam;

   return *m_WindowConfig;
}

void GeometryRenderer::SetLight( Light& const light )
{
   if (m_Light)
   {
      Debug::Instance().LogError( "Light set twice" );
   }

   m_Light = &light;
}

void GeometryRenderer::Render()
{
   for (MeshNameToShaderNameToRenderSlot::iterator it = m_RenderingSlots.begin; it != m_RenderingSlots.end; it++)
   {
      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      meshNameToSlot.begin()->second->UseProgram();

      m_Camera->ApplyCameraUniforms();
      m_Light->ApplyLightUniforms();

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin(); it2 != meshNameToSlot.end(); it2++)
      {
         it2->second->BindMesh();
         it2->second->Render(); // the number of vertices to draw is equal to the number of indices
      }
   }
}