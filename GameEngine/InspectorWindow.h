#pragma once

class GameObject;

class InspectorWindow
{
public:
   InspectorWindow();

   void SetActiveGameObject( GameObject * obj );
   void Update();

private:
   GameObject* m_ActiveGameObject;
};
