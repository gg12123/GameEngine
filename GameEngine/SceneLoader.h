#pragma once
#include <string>
#include <vector>

class World;
class GameObject;

class ISceneLoader
{
public:
   virtual void PendLoadScene( std::string name ) = 0; // this is whats available to the components
};

class SceneLoader : public ISceneLoader
{
public:
   SceneLoader();

   void LoadStartUpScene();
   void LoadCompletelyNewScene( std::string name );
   bool LoadScene( std::string name );
   void Init( World& world );
   void Update();

   std::string GetActiveSceneName();

   void PendLoadScene( std::string name ) override;

protected:
   virtual void CallAwake( World& world, GameObject& rootGameObject, std::vector<GameObject*>& gameObjects ) = 0;

private:
   World *m_World;
   std::string m_PendingSceneName;
   std::string m_ActiveSceneName;
   bool m_LoadPending;
};

class EditModeSceneLoader : public SceneLoader
{
protected:
   void CallAwake( World& world, GameObject& rootGameObject, std::vector<GameObject*>& gameObjects ) override;
};

class PlayModeSceneLoader : public SceneLoader
{
protected:
   void CallAwake( World& world, GameObject& rootGameObject, std::vector<GameObject*>& gameObjects ) override;
};
