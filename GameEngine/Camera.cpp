#include "Camera.h"
#include "ShaderLocations.h"

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

void Camera::Awake()
{
   m_WindowConfig = &GetGeometryRenderer().SetCamera( *this );
}

void Camera::ApplyCameraUniforms()
{
   mat4 cameraTransform = GetGameObject().GetTransfrom().GetTransformMatrixAssumingClean();

   vec3 cameraForward = vec3( cameraTransform[ 2 ][ 0 ], cameraTransform[ 2 ][ 1 ], cameraTransform[ 2 ][ 2 ] );
   vec3 cameraUp = vec3( cameraTransform[ 1 ][ 0 ], cameraTransform[ 1 ][ 1 ], cameraTransform[ 1 ][ 2 ] );
   vec3 cameraRight = vec3( cameraTransform[ 0 ][ 0 ], cameraTransform[ 0 ][ 1 ], cameraTransform[ 0 ][ 2 ] );

   vec3 cameraPos = vec3( cameraTransform[ 3 ][ 0 ], cameraTransform[ 3 ][ 1 ], cameraTransform[ 3 ][ 2 ] );

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