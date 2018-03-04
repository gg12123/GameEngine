#include "GeometryRenderer.h"
#include "GL/gl3w.h"
#include "Debug.h"
#include "Vertex.h"
#include "ShaderLocations.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Light.h"

GeometryRenderer::~GeometryRenderer()
{
   for (ShaderNameToMeshNameToRenderSlot::iterator it = m_RenderingSlots.begin(); it != m_RenderingSlots.end(); it++)
   {
      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin(); it2 != meshNameToSlot.end(); it2++)
      {
         // delete each render slot
         delete it2->second;
      }

      // delete the map from mesh name to render slot
      delete it->second;
   }

   m_Camera = nullptr;
   m_WindowConfig = nullptr;
   m_AssetLoader = nullptr;
   m_Light = nullptr;
}

void GeometryRenderer::OnDestroy()
{
   for (ShaderNameToMeshNameToRenderSlot::iterator it = m_RenderingSlots.begin(); it != m_RenderingSlots.end(); it++)
   {
      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin(); it2 != meshNameToSlot.end(); it2++)
      {
         it2->second->OnDestroy();
      }
   }
}

GeometryRenderer::GeometryRenderer()
{
   m_Camera = nullptr;
   m_WindowConfig = nullptr;
   m_AssetLoader = nullptr;
   m_Light = nullptr;
}

void GeometryRenderer::Awake( IWindowConfiguration& windowConfig, AssetLoader& assetLoader )
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

   // Enable depth test
   glEnable( GL_DEPTH_TEST );
   glDepthFunc( GL_LEQUAL );
}

std::list<MeshRenderer*>::iterator GeometryRenderer::Register( MeshRenderer& toReg )
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

IWindowConfiguration& GeometryRenderer::SetCamera( Camera& cam )
{
   if (m_Camera)
   {
      Debug::Instance().LogError( "Camera set twice" );
   }

   m_Camera = &cam;

   return *m_WindowConfig;
}

void GeometryRenderer::SetLight( Light& light )
{
   if (m_Light)
   {
      Debug::Instance().LogError( "Light set twice" );
   }

   m_Light = &light;
}

void GeometryRenderer::Render()
{
   // Clear background and depth
   const GLfloat one = 1.0f;
   const GLfloat background_colour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
   glClearBufferfv( GL_COLOR, 0, background_colour );
   glClearBufferfv( GL_DEPTH, 0, &one );

   for (ShaderNameToMeshNameToRenderSlot::iterator it = m_RenderingSlots.begin(); it != m_RenderingSlots.end(); it++)
   {
      MeshNameToRenderSlot &meshNameToSlot = *(it->second);

      meshNameToSlot.begin()->second->UseProgram();

      m_Camera->ApplyCameraUniforms();
      m_Light->ApplyLightUniforms();

      for (MeshNameToRenderSlot::iterator it2 = meshNameToSlot.begin(); it2 != meshNameToSlot.end(); it2++)
      {
         it2->second->BindMesh();
         it2->second->Render();
      }
   }
}