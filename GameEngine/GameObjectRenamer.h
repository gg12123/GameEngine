#pragma once

class GameObject;

#define MAX_GO_NAME_SIZE 50

class GameObjectRenamer
{
public:
   void OnGUI( GameObject& active );
private:
   char m_Buffer[ MAX_GO_NAME_SIZE ];
};
