#pragma once
#include "BehaviourComponent.h"
#include "TransformHandle.h"
#include "Events.h"

class TransformHandles : public BehaviourComponent
{
public:
   TransformHandles();

   void EditUpdate() override;
   int32_t GetType() override;

protected:
   void EditAwake( IEditor& editor ) override;

private:
   bool IsHandle( GameObject& obj );
   bool TryGetHandle();

   std::vector<TransformHandle*> m_Handles;
   TransformHandle* m_ActiveHandle;
   IEditor* m_Editor;
};

