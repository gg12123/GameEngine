#include "Camera.h"
#include "ShaderLocations.h"
#include "GeometryRenderer.h"
#include "MyVmath.h"
#include "Transform.h"
#include "GameObject.h"
#include "ComponentIDs.h"
#include "Utils.h"

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

mat4 Camera::ConstructWorldToView( const mat4& cameraTransform )
{
   vec3 cameraForward = extractForwardOnly( cameraTransform );
   vec3 cameraUp = extractUpOnly( cameraTransform );
   vec3 cameraRight = extractRightOnly( cameraTransform );
   vec3 cameraPos = extractPositionOnly( cameraTransform );

   mat4 worldToView;

   worldToView[ 0 ] = vec4( cameraRight[ 0 ], cameraUp[ 0 ], cameraForward[ 0 ], 0.0f );
   worldToView[ 1 ] = vec4( cameraRight[ 1 ], cameraUp[ 1 ], cameraForward[ 1 ], 0.0f );
   worldToView[ 2 ] = vec4( cameraRight[ 2 ], cameraUp[ 2 ], cameraForward[ 2 ], 0.0f );
   worldToView[ 3 ] = vec4( -dot( cameraPos, cameraRight ), -dot( cameraPos, cameraUp ), -dot( cameraPos, cameraForward ), 1.0f );

   return worldToView;
}

mat4 Camera::ConstructPerspectiveProj()
{
   mat4 persp = perspective2( m_FOVAngle.Value(),
                              m_WindowConfig->GetWidth() / m_WindowConfig->GetHeight(),
                              m_NearClip.Value(),
                              m_FarClip.Value() );

   return persp;
}

void Camera::ApplyCameraUniforms()
{
   mat4 cameraTransform = GetGameObject().GetTransform().GetTransformMatrixAssumingClean();

   mat4 wvp = ConstructPerspectiveProj() * ConstructWorldToView( cameraTransform );

   glUniform3fv( CAMERA_DIRECTION_LOCATION,
                 1,
                 extractForwardOnly( cameraTransform ) );

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

void Camera::MakeActive()
{
   m_WindowConfig = &GetGeometryRenderer().SetCamera( *this );
}

bool Camera::IsActive()
{
   return (&GetActiveCamera() == this);
}

Ray Camera::ScreenPointToRay( const vec2& screenPoint )
{
   const float w = m_WindowConfig->GetWidth() / 2.0f;
   const float h = m_WindowConfig->GetWidth() / 2.0f;

   TwoDimentionalSpace screenSpace( w,
                                    h,
                                    vec2( w, h ) );

   TwoDimentionalSpace camPlaneSpace( 1.0f,
                                      1.0f,
                                      vec2( 0.0f, 0.0f ) );

   vec2 pointInCamPlane = Transform2DPoint( screenSpace, camPlaneSpace, screenPoint );

   mat4 cameraTransform = GetGameObject().GetTransform().GetTransformMatrix();

   vec3 pointInCamPlaneGlobal = (1.0f / tanf( 0.5f * m_FOVAngle.Value() )) * extractForwardOnly( cameraTransform ) +
      pointInCamPlane[ 0 ] * extractRightOnly( cameraTransform ) +
      pointInCamPlane[ 1 ] * extractUpOnly( cameraTransform );

   vec3 cameraPos = extractPositionOnly( cameraTransform );

   return Ray( normalize( pointInCamPlaneGlobal - cameraPos ), cameraPos );
}

vmath::vec2 Camera::ToScreenSpaceDirection( Ray& globalRay )
{
   vec4 origin = vec4( globalRay.GetOrigin(), 1.0f );
   vec4 end = vec4( globalRay.GetOrigin() + globalRay.GetDirection(), 1.0f );

   mat4 wvp = ConstructPerspectiveProj() * ConstructWorldToView( GetGameObject().GetTransform().GetTransformMatrix() );

   vec4 originP = wvp * origin;
   vec4 endP = wvp * end;

   for (int i = 0; i < 3; i++)
   {
      originP[ i ] /= originP[ 3 ];
      endP[ i ] /= endP[ 3 ];
   }

   vec4 dirProj = endP - originP;
   vec2 dirProj2 = vec2( dirProj[ 0 ], dirProj[ 1 ] );

   const float w = m_WindowConfig->GetWidth() / 2.0f;
   const float h = m_WindowConfig->GetWidth() / 2.0f;

   TwoDimentionalSpace screenSpace( w,
                                    h,
                                    vec2( w, h ) );

   TwoDimentionalSpace camPlaneSpace( 1.0f,
                                      1.0f,
                                      vec2( 0.0f, 0.0f ) );

   return Transform2DDirection( camPlaneSpace, screenSpace, dirProj2 );
}