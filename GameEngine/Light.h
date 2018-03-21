#pragma once
#include "EngineComponent.h"

class Light : public EngineComponent
{
public:
   void ApplyLightUniforms();
   int32_t GetType() override;

protected:
   void Awake() override;
   void EditAwake( IEditor& editor ) override;
};
