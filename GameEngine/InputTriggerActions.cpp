#include <vcruntime_exception.h>
#include "InputTriggerActions.h"
#include "InputTrigger.h"


void InputTriggerDown::OnInput( InputTrigger& owner )
{
   owner.m_IsTriggeredDown = true;
   owner.m_IsTriggered = true;
}

void InputTriggerDown::Clear( InputTrigger& owner )
{
   owner.m_IsTriggeredDown = false;
}

void InputTriggerUp::OnInput( InputTrigger& owner )
{
   owner.m_IsTriggeredUp = true;
}

void InputTriggerUp::Clear( InputTrigger& owner )
{
   owner.m_IsTriggeredUp = false;
   owner.m_IsTriggered = false;
}

void NullInputTriggerAction::OnInput( InputTrigger& owner )
{
   throw std::exception( "Null action - on input called" );
}

void NullInputTriggerAction::Clear( InputTrigger& owner )
{
   throw std::exception( "Null action - clear called" );
}