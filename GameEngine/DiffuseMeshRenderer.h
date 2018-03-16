#pragma once
#include "MeshRenderer.h"
#include "SerializedFields.h"

class DiffuseMeshRenderer : public MeshRenderer
{
public:
   DiffuseMeshRenderer();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;

   int32_t GetType() override;

protected:
   virtual void ApplyUniforms() override;

private:
   SerializedVector4 m_Colour;
};
