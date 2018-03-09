#pragma once
#include "Component.h"
#include "SerializedFields.h"
#include "MeshField.h"
#include "NullableIterator.h"
#include "Events.h"
#include "GameObjectEvents.h"

class MeshRenderer : public Component
{
public:

   MeshRenderer();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;

   void OnDestroy();

   // this will need to be virtual
   void Render(const int count);

   std::string GetMeshName();
   std::string GetShaderName();

   int32_t GetType() override;

   // At the moment, setting these will only have an effect if its done before awake.
   void SetMeshName( std::string name );
   void SetShaderName( std::string name );

protected:

   void Awake() override;
   void EditAwake( IEditor& editor ) override;
   virtual void ApplyUniforms();

private:

   MeshField m_MeshName;
   ShaderField m_ShaderName;
   NullableValue<std::list<MeshRenderer*>::iterator> m_ToThisInRenderersList;

   VoidEventHandler<MeshRenderer> m_OnDestroyEvent;
};
