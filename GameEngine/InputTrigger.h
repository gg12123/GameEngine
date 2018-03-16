#pragma once
#include "InputTriggerActions.h"

class InputTrigger
{
   friend class InputTriggerDown;
   friend class InputTriggerUp;

public:

   InputTrigger();
   void OnDown();
   void OnUp();
   void Clear();

   // returns true the whole time the trigger is triggered.
   bool IsTriggerd() const;

   // retruns true on the frame that the trigger goes down.
   bool IsTriggerdDown() const;

   // returns true on the frame that the trigger goes up.
   bool IsTriggeredUp() const;

private:
   InputTriggerAction* m_ActiveAction;

   bool m_IsTriggered;
   bool m_IsTriggeredDown;
   bool m_IsTriggeredUp;

   InputTriggerDown m_TriggerDownAction;
   InputTriggerUp m_TriggerUpAction;
   NullInputTriggerAction m_NullAction;
};
