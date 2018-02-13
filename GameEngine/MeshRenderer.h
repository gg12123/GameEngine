#pragma once
#include "Component.h"
#include "SerializedFields.h"

class MeshRenderer : public Component
{
public:

   void GetSerializedFields( std::map<std::string, SerializedField*>& fields );

   std::string GetMeshName();
   std::string GetShaderName();

protected:

   void Awake();

private:

   SerializedString m_MeshName;
   SerializedString m_ShaderName;
};
