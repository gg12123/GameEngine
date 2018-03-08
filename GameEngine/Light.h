#pragma once
#include "Component.h"

class Light : public Component
{
public:
   void ApplyLightUniforms();
   int32_t GetType() override;

protected:
   void Awake() override;
   void EditAwake() override;
};
