#include "MeshRenderer.h"
#include "GL/gl3w.h"
#include "ShaderLocations.h"

void MeshRenderer::Awake()
{
   // register with the geometry renderer
}

void MeshRenderer::GetSerializedFields( std::unordered_map<std::string, SerializedField*> &fields )
{
   fields[ "shaderName" ] = &m_ShaderName;
   fields[ "meshName" ] = &m_MeshName;
}

void MeshRenderer::Render(const int count)
{
   glUniformMatrix4fv( OW_MATRIX_LOCATION,
                       1,
                       GL_FALSE,
                       GetGameObject().GetTransfrom().GetTransformMatrix() );

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