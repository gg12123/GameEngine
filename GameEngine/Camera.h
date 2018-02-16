#pragma once
#include "Component.h"
#include "WindowConfiguration.h"

class Camera : public Component
{
public:

   Camera();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void ApplyCameraUniforms();

protected:

   void Awake() override;

private:

   // will need to pull aspect ratio in from world
   SerializedFloat m_NearClip;
   SerializedFloat m_FarClip;
   SerializedFloat m_FOVAngle;
   WindowConfiguration *m_WindowConfig;
};
