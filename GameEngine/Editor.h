#pragma once
#include <vector>
#include <unordered_map>
#include "EditorEvents.h"

class World;
class GameObject;
class EditorWindow;

class Editor
{
public:

   Editor();

   void Awake( World& w );
   void Update();

   void SetActiveGameObject( GameObject* active );
   GameObject* GetActiveGameObject();

   World& GetWorld();

   void AddWindow( EditorWindow& wnd );

   void RegisterForEvent( EEditorEvent eventID, EditorEventFunctionPtr callback );
   void UnregisterCallback( EEditorEvent eventID, EditorEventFunctionPtr callback );

   void InvokeEvent( EEditorEvent eventID );

private:
   World* m_World;
   GameObject* m_ActiveGameObject;
   std::vector<EditorWindow*> m_CurrentWindows;
   std::unordered_map<EEditorEvent, std::vector<EditorEventFunctionPtr>*> m_Events;
};
