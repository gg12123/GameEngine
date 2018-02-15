#pragma once
#include "Component.h"


class Camera : public Component
{
public:

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void ApplyUniformsToShader();

protected:

   void Awake() override;

private:

   // will need to pull aspect ratio in from world
   SerializedFloat m_NearClip;
   SerializedFloat m_FarClip;
   SerializedFloat m_FOVAngle;
};
