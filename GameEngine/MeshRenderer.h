#pragma once
#include "Component.h"
#include "SerializedFields.h"

class MeshRenderer : public Component
{
public:

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;

   // this will need to be virtual
   void Render(const int count);

   std::string GetMeshName();
   std::string GetShaderName();

protected:

   void Awake() override;

private:

   SerializedString m_MeshName;
   SerializedString m_ShaderName;
};
