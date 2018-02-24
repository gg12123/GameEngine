#include "Light.h"
#include "ShaderLocations.h"
#include "GeometryRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "MyVmath.h"
#include "ComponentIDs.h"

using namespace vmath;

void Light::Awake()
{
   GetGeometryRenderer().SetLight( *this );
}

void Light::ApplyLightUniforms()
{
   mat4 lightTransform = GetGameObject().GetTransform().GetTransformMatrixAssumingClean();
   vec3 lightDir = vec3( lightTransform[ 2 ][ 0 ], lightTransform[ 2 ][ 1 ], lightTransform[ 2 ][ 2 ] );

   glUniform3fv( LIGHT_DIRECTION_LOCATION,
                 1,
                 lightDir );
}

int32_t Light::GetType()
{
   return COMPONENT_ID_LIGHT;
}

std::string Light::GetName()
{
   return "Light";
}