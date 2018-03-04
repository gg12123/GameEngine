#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include "EditorEvents.h"
#include "Events.h"

class World;
class GameObject;
class EditorWindow;

class Editor
{
public:

   ~Editor();
   Editor();

   void Awake( World& w );
   void Update();

   void SetActiveGameObject( GameObject* active );
   GameObject* GetActiveGameObject();

   World& GetWorld();

   void AddWindow( EditorWindow& wnd );

   void RegisterCallbackForEvent( EEditorEvent eventID, Event& callback );
   void UnregisterCallback( EEditorEvent eventID, Event& callback );

   void InvokeEvent( EEditorEvent eventID );

private:
   World* m_World;
   GameObject* m_ActiveGameObject;
   std::vector<EditorWindow*> m_CurrentWindows;
   std::unordered_map<EEditorEvent, std::vector<Event*>*> m_Events;
};
