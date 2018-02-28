#pragma once

class GameObject;

class InspectorWindow
{
public:

   InspectorWindow();

   void SetActiveGameObject( GameObject * obj );
   void OnGUI();

private:
   GameObject* m_ActiveGameObject;
};
