#pragma once
#include "Component.h"
#include "SerializedFields.h"

class MeshRenderer : public Component
{
public:

   MeshRenderer();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;

   // this will need to be virtual
   void Render(const int count);

   std::string GetMeshName();
   std::string GetShaderName();

   int32_t GetType() override;
   std::string GetName() override;

   // At the moment, setting these will only have an effect if its done before awake.
   void SetMeshName( std::string name );
   void SetShaderName( std::string name );

protected:

   void Awake() override;
   virtual void ApplyUniforms();

private:

   SerializedString m_MeshName;
   SerializedString m_ShaderName;
   std::list<MeshRenderer*>::iterator m_ThisInRenderersList;
};
