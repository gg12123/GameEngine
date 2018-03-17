#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include "EditorEvents.h"
#include "EventManager.h"
#include "IEditor.h"

class World;
class GameObject;
class EditorWindow;

class Editor : public IEditor
{
public:

   ~Editor();
   Editor();

   void Init( World& w );
   void Update();

   void SetActiveGameObject( GameObject* active ) override;
   GameObject* GetActiveGameObject() override;

   void RegisterCallbackForEvent( EEditorEvent eventID, EventHandler& callback ) override;
   void InvokeEvent( EEditorEvent eventID ) override;

   World& GetWorld();

   void AddWindow( EditorWindow& wnd );

private:
   void OnHierarchyBeingDestroyed( GameObject& root );

   World* m_World;
   GameObject* m_ActiveGameObject;
   std::vector<EditorWindow*> m_CurrentWindows;
   EventManager<EEditorEvent, NUMBER_OF_EDITOR_EVENTS> m_Events;

   GOEventHandler<Editor> m_OnHierarchyBeingDestroyedEvent;
};
