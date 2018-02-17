#pragma once
#include "Component.h"

class Light : public Component
{
public:
   void Awake() override;
   void ApplyLightUniforms();
};
