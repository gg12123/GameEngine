#pragma once
#include "Component.h"
#include "TransformHandle.h"
#include "Events.h"

class TransformHandles : public Component
{
public:
   TransformHandles();

   void EditUpdate() override;
   int32_t GetType() override;

protected:
   void EditAwake( IEditor& editor ) override;

private:
   void OnNewActiveGO();
   bool IsHandle( GameObject& obj );
   void TryToStartMovement( GameObject& active );

   std::vector<TransformHandle*> m_Handles;
   TransformHandle* m_ActiveHandle;
   VoidEventHandler<TransformHandles> m_OnActiveGOChangedEvent;
   IEditor* m_Editor;
};

