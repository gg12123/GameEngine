#pragma once
#include <string>
#include <vector>

class World;
class GameObject;
class Editor;

class SceneLoader
{
public:
   void LoadStartUpScene();
   void LoadCompletelyNewScene( std::string name );
   bool LoadScene( std::string name );
   void PendLoadScene( std::string name );

   void Update();

   std::string GetActiveSceneName();

protected:
   virtual void CallAwake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects ) = 0;
   virtual void ClearAll() = 0;

private:
   std::string m_PendingSceneName;
   std::string m_ActiveSceneName;
   bool m_LoadPending;
};

class EditModeSceneLoader : public SceneLoader
{
public:
   EditModeSceneLoader();
   void Init( Editor& editor );
protected:
   void CallAwake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects ) override;
   void ClearAll() override;
private:
   Editor* m_Editor;
};

class PlayModeSceneLoader : public SceneLoader
{
public:
   PlayModeSceneLoader();
   void Init( World& world );
protected:
   void CallAwake( GameObject& rootGameObject, std::vector<GameObject*>& gameObjects ) override;
   void ClearAll() override;
private:
   World* m_World;
};
