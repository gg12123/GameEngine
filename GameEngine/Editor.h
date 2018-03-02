#pragma once
#include <vector>

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

private:
   World* m_World;
   GameObject* m_ActiveGameObject;
   std::vector<EditorWindow*> m_CurrentWindows;
};
