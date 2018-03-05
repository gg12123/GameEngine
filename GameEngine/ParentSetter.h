#pragma once

class GameObject;

class ParentSetter
{
public:
   ParentSetter();
   void Draw( GameObject& gameObject );
   void OnEndOfDrawing();

private:
   GameObject *m_PotentialParent;
   GameObject *m_PotentialChild;
};