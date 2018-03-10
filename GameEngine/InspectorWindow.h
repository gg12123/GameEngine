#pragma once
#include <unordered_map>
#include <string>
#include "EditorWindow.h"
#include "Events.h"

class GameObject;
class SerializedField;
class Component;

typedef void( *InspectorGUIFunctionPtr1 )(std::vector<Component*>::iterator);
typedef void( *InspectorGUIFunctionPtr2 )(std::unordered_map<std::string, SerializedField*>::iterator, Component&);

class InspectorWindow : public EditorWindow
{
public:
   InspectorWindow();

   void Awake( Editor& editor ) override;
   void Update() override;

   void CheckForActiveGOChanged();

private:
   void OnSceneLoaded();

   void CallIntoSerializedFields( GameObject* active,
                                  InspectorGUIFunctionPtr1 function1,
                                  InspectorGUIFunctionPtr2 function2 );

   Editor* m_Editor;
   GameObject *m_PrevActiveGO;
   VoidEventHandler<InspectorWindow> m_OnSceneLoadedEvent;
};
