#include "MeshRenderer.h"
#include "GeometryRenderer.h"
#include "GL/gl3w.h"
#include "ShaderLocations.h"
#include "GameObject.h"
#include "Transform.h"
#include "ComponentIDs.h"

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::OnDestroy()
{
   if (!m_ToThisInRenderersList.IsNull())
   {
      GetGeometryRenderer().UnRegister( *this, m_ToThisInRenderersList.Get() );
   }
}

void MeshRenderer::EditAwake()
{
   Awake();
}

void MeshRenderer::Awake()
{
   m_ToThisInRenderersList.Set( GetGeometryRenderer().Register( *this ) );
   GetGameObject().RegisterForEvent( eOnDestroy, *m_OnDestroyEvent.Init( &MeshRenderer::OnDestroy, this ) );
}

void MeshRenderer::GetSerializedFields( std::unordered_map<std::string, SerializedField*> &fields )
{
   fields[ "shaderName" ] = &m_ShaderName;
   fields[ "meshName" ] = &m_MeshName;
}

void MeshRenderer::ApplyUniforms()
{
   glUniformMatrix4fv( OW_MATRIX_LOCATION,
                       1,
                       GL_FALSE,
                       GetGameObject().GetTransform().GetTransformMatrixAssumingClean() );

   // Should set this in a derived class
   glUniform4fv( COLOUR1_LOCATION,
                 1,
                 vec4(0.0f, 1.0f, 0.0f, 1.0f) );
}

void MeshRenderer::Render(const int count)
{
   ApplyUniforms();

   glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0 );
}

std::string MeshRenderer::GetMeshName()
{
   return m_MeshName.Value();
}

std::string MeshRenderer::GetShaderName()
{
   return m_ShaderName.Value();
}

int32_t MeshRenderer::GetType()
{
   return COMPONENT_ID_MESHRENDERER;
}

void MeshRenderer::SetMeshName( std::string name )
{
   m_MeshName.SetValue( name );
}

void MeshRenderer::SetShaderName( std::string name )
{
   m_ShaderName.SetValue( name );
}
