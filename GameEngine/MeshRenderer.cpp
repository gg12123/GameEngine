#include "MeshRenderer.h"

void MeshRenderer::Awake()
{
   // register with the geometry renderer
}

void MeshRenderer::GetSerializedFields( std::map<std::string, SerializedField*> &fields )
{
   fields[ "shaderName" ] = &m_ShaderName;
}