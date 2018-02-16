#include "MeshRenderer.h"
#include "GL/gl3w.h"
#include "ShaderLocations.h"

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::Awake()
{
   m_ThisInRenderersList = GetGeometryRenderer().Register( *this );
}

void MeshRenderer::GetSerializedFields( std::unordered_map<std::string, SerializedField*> &fields )
{
   fields[ "shaderName" ] = &m_ShaderName;
   fields[ "meshName" ] = &m_MeshName;
}

void MeshRenderer::ApplyUniforms()
{

}

void MeshRenderer::Render(const int count)
{
   glUniformMatrix4fv( OW_MATRIX_LOCATION,
                       1,
                       GL_FALSE,
                       GetGameObject().GetTransfrom().GetTransformMatrixAssumingClean() );

   glDrawElements( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0 );
}

std::string MeshRenderer::GetMeshName()
{
   return m_MeshName.Value();
}

std::string MeshRenderer::GetShaderName()
{
   return m_MeshName.Value();
}