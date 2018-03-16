#pragma once
#include "Component.h"
#include "WindowConfiguration.h"
#include "Ray.h"

class Camera : public Component
{
public:

   Camera();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void ApplyCameraUniforms();

   int32_t GetType() override;

   void SetNearClip( float val );
   void SetFarClip( float val );
   void SetFOV( float val );

   void MakeActive();
   bool IsActive();

   vmath::vec2 ToScreenSpaceDirection( Ray& globalRay );
   Ray ScreenPointToRay( const vmath::vec2& screenPoint );

   vmath::mat4 ConstructWorldToView( const vmath::mat4& cameraTransform );
   vmath::mat4 ConstructPerspectiveProj();

protected:

   void Awake() override;
   void EditAwake( IEditor& editor ) override;

private:
   bool m_IsActive;

   // will need to pull aspect ratio in from world
   SerializedFloat m_NearClip;
   SerializedFloat m_FarClip;
   SerializedFloat m_FOVAngle;
   IWindowConfiguration *m_WindowConfig;
};
