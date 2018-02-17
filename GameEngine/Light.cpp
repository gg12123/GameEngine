#include "Light.h"
#include "ShaderLocations.h"

void Light::Awake()
{
   GetGeometryRenderer().SetLight( *this );
}

void Light::ApplyLightUniforms()
{
   mat4 lightTransform = GetGameObject().GetTransfrom().GetTransformMatrixAssumingClean();
   vec3 lightDir = vec3( lightTransform[ 2 ][ 0 ], lightTransform[ 2 ][ 1 ], lightTransform[ 2 ][ 2 ] );

   glUniform3fv( LIGHT_DIRECTION_LOCATION,
                 1,
                 lightDir );
}