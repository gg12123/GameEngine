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

void Light::EditAwake()
{
   Awake();
}

void Light::ApplyLightUniforms()
{
   mat4 lightTransform = GetGameObject().GetTransform().GetTransformMatrixAssumingClean();
   vec3 lightDir = extractForwardOnly( lightTransform );

   glUniform3fv( LIGHT_DIRECTION_LOCATION,
                 1,
                 lightDir );
}

int32_t Light::GetType()
{
   return COMPONENT_ID_LIGHT;
}