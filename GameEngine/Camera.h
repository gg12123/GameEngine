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

   static Camera& Active();
   void MakeActive();
   bool IsActive() const;

   vmath::vec2 ToScreenSpaceDirection( const vmath::vec3& globalDir ) const;
   Ray ScreenPointToRay( const vmath::vec2& screenPoint ) const;

protected:

   void Awake() override;
   void EditAwake( IEditor& editor ) override;

private:

   // will need to pull aspect ratio in from world
   SerializedFloat m_NearClip;
   SerializedFloat m_FarClip;
   SerializedFloat m_FOVAngle;
   IWindowConfiguration *m_WindowConfig;

   static Camera* m_Active;
};
