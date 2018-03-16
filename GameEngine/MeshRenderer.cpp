#include "MeshRenderer.h"
#include "GeometryRenderer.h"
#include "GL/gl3w.h"
#include "ShaderLocations.h"
#include "GameObject.h"
#include "Transform.h"
#include "ComponentIDs.h"
#include "AssetLoader.h"

MeshRenderer::MeshRenderer()
{
   m_Mesh = nullptr;
}

void MeshRenderer::OnDestroy()
{
   if (!m_ToThisInRenderersList.IsNull())
   {
      GetGeometryRenderer().UnRegister( *this, m_ToThisInRenderersList.Get() );
   }
}

void MeshRenderer::EditAwake( IEditor& editor )
{
   Awake();
}

void MeshRenderer::Awake()
{
   m_Mesh = &GetAssetLoader().LoadIfNotAlreadyLoaded( GetMeshName(), Mesh::CreateInstance ).MeshValue();
   m_ToThisInRenderersList.Set( GetGeometryRenderer().Register( *this ) );
   GetGameObject().RegisterForEvent( eOnDestroy, *m_OnDestroyEvent.Init( &MeshRenderer::OnDestroy, this ) );
}

void MeshRenderer::GetSerializedFields( std::unordered_map<std::string, SerializedField*> &fields )
{
   fields[ "shaderName" ] = &m_ShaderName;
   fields[ "meshName" ] = &m_MeshName;
}

void MeshRenderer::Render(const int count)
{
   ApplyUniforms();

   glUniformMatrix4fv( OW_MATRIX_LOCATION,
                       1,
                       GL_FALSE,
                       GetGameObject().GetTransform().GetTransformMatrixAssumingClean() );

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

void MeshRenderer::SetMeshName( std::string name )
{
   m_MeshName.SetValue( name );
   m_Mesh = &GetAssetLoader().LoadIfNotAlreadyLoaded( name, Mesh::CreateInstance ).MeshValue();
   // re-register
}

void MeshRenderer::SetShaderName( std::string name )
{
   m_ShaderName.SetValue( name );
   // re-register
}

void MeshRenderer::EnsureMeshIsNotShared()
{
   SetMeshName( InstantiateMesh( m_MeshName ) );
}

Mesh& MeshRenderer::GetMeshData()
{
   if (!m_Mesh)
   {
      m_Mesh = &GetAssetLoader().LoadIfNotAlreadyLoaded( GetMeshName(), Mesh::CreateInstance ).MeshValue();
   }

   return *m_Mesh;
}
