#pragma once
#include "Component.h"
#include "WindowConfiguration.h"

class Camera : public Component
{
public:

   Camera();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void ApplyCameraUniforms();

   int32_t GetType() override;
   std::string GetName() override;

protected:

   void Awake() override;

private:

   // will need to pull aspect ratio in from world
   SerializedFloat m_NearClip;
   SerializedFloat m_FarClip;
   SerializedFloat m_FOVAngle;
   IWindowConfiguration *m_WindowConfig;
};
