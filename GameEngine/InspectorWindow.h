#pragma once
#include <unordered_map>
#include <string>
#include "EditorWindow.h"
#include "Events.h"

class GameObject;
class SerializedField;

typedef void( *InspectorGUIFunctionPtr )(std::unordered_map<std::string, SerializedField*>::iterator);

class InspectorWindow : public EditorWindow
{
public:
   InspectorWindow();

   void Awake( Editor& editor ) override;
   void Update() override;

   void OnActiveGameObjectChanged();

private:
   void CallIntoSerializedFields( GameObject* active, InspectorGUIFunctionPtr function );

   Editor* m_Editor;
   Event* m_OnNewActiveGOEvent;
};
