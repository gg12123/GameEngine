#include "DiffuseMeshRenderer.h"
#include "GL/gl3w.h"
#include "ShaderLocations.h"
#include "ComponentIDs.h"

DiffuseMeshRenderer::DiffuseMeshRenderer()
{
   m_Colour.SetValue( vmath::vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
}

void DiffuseMeshRenderer::GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields )
{
   MeshRenderer::GetSerializedFields( fields );
   fields[ "colour" ] = &m_Colour;
}

void DiffuseMeshRenderer::SetColour( vmath::vec4 colur )
{
   m_Colour.SetValue( colur );
}

void DiffuseMeshRenderer::ApplyUniforms()
{
   glUniform4fv( COLOUR1_LOCATION,
                 1,
                 m_Colour.Value() );
}

int32_t DiffuseMeshRenderer::GetType()
{
   return COMPONENT_ID_DIFFUSEMESHRENDERER;
}