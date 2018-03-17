#pragma once
#include "EditorEvents.h"

class GameObject;
class EventHandler;

class IEditor
{
public:
   virtual void SetActiveGameObject( GameObject* active ) = 0;
   virtual GameObject* GetActiveGameObject() = 0;
   virtual void RegisterCallbackForEvent( EEditorEvent eventID, EventHandler& callback ) = 0;
   virtual void InvokeEvent( EEditorEvent eventID ) = 0;
};
