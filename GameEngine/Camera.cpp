#include "Camera.h"
#include "ShaderLocations.h"
#include "GeometryRenderer.h"
#include "MyVmath.h"
#include "Transform.h"
#include "GameObject.h"
#include "ComponentIDs.h"

using namespace vmath;

Camera::Camera()
{
   m_WindowConfig = nullptr;
}

void Camera::GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields )
{
   fields[ "fov" ] = &m_FOVAngle;
   fields[ "nearClip" ] = &m_NearClip;
   fields[ "farClip" ] = &m_FarClip;
}

void Camera::EditAwake( IEditor& editor )
{
   Awake();
}

void Camera::Awake()
{
   MakeActive();
}

void Camera::ApplyCameraUniforms()
{
   mat4 cameraTransform = GetGameObject().GetTransform().GetTransformMatrixAssumingClean();

   vec3 cameraForward = extractForwardOnly( cameraTransform );
   vec3 cameraUp = extractUpOnly( cameraTransform );
   vec3 cameraRight = extractRightOnly( cameraTransform );
   vec3 cameraPos = extractPositionOnly( cameraTransform );

   mat4 worldToView;

   worldToView[ 0 ] = vec4( cameraRight[ 0 ], cameraUp[ 0 ], cameraForward[ 0 ], 0.0f );
   worldToView[ 1 ] = vec4( cameraRight[ 1 ], cameraUp[ 1 ], cameraForward[ 1 ], 0.0f );
   worldToView[ 2 ] = vec4( cameraRight[ 2 ], cameraUp[ 2 ], cameraForward[ 2 ], 0.0f );
   worldToView[ 3 ] = vec4( -dot( cameraPos, cameraRight ), -dot( cameraPos, cameraUp ), -dot( cameraPos, cameraForward ), 1.0f );

   mat4 persp = perspective2( m_FOVAngle.Value(),
                              m_WindowConfig->GetWidth() / m_WindowConfig->GetHeight(),
                              m_NearClip.Value(),
                              m_FarClip.Value() );

   mat4 wvp = persp * worldToView;

   glUniform3fv( CAMERA_DIRECTION_LOCATION,
                 1,
                 cameraForward );

   glUniformMatrix4fv( WVP_MATRIX_LOCATION,
                       1,
                       GL_FALSE,
                       wvp );
}

int32_t Camera::GetType()
{
   return COMPONENT_ID_CAMERA;
}

void Camera::SetNearClip( float val )
{
   m_NearClip.SetValue( val );
}

void Camera::SetFarClip( float val )
{
   m_FarClip.SetValue( val );
}

void Camera::SetFOV( float val )
{
   m_FOVAngle.SetValue( val );
}

Camera& Camera::Active()
{
   return *m_Active;
}

void Camera::MakeActive()
{
   m_WindowConfig = &GetGeometryRenderer().SetCamera( *this );
   m_Active = this;
}

bool Camera::IsActive() const
{
   return (m_Active == this);
}