#include <vcruntime_exception.h>
#include "InputTrigger.h"

InputTrigger::InputTrigger()
{
   m_IsTriggered = false;
   m_IsTriggeredDown = false;
   m_IsTriggeredUp = false;

   m_ActiveAction = &m_NullAction;
}

void InputTrigger::OnDown()
{
   m_ActiveAction = &m_TriggerDownAction;
   m_ActiveAction->OnInput( *this );
}

void InputTrigger::OnUp()
{
   m_ActiveAction = &m_TriggerUpAction;
   m_ActiveAction->OnInput( *this );
}

void InputTrigger::Clear()
{
   m_ActiveAction->Clear( *this );
   m_ActiveAction = &m_NullAction;
}

bool InputTrigger::IsTriggerd() const
{
   return m_IsTriggered;
}

bool InputTrigger::IsTriggerdDown() const
{
   return m_IsTriggeredDown;
}

bool InputTrigger::IsTriggeredUp() const
{
   return m_IsTriggeredUp;
}