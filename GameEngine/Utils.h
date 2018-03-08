#pragma once
#include <stack>
#include <string>
#include "MyVmath.h"

class GameObject;
class Transform;

class EnumerableHierarchy
{
public:
   EnumerableHierarchy( GameObject& root );

   GameObject* Next();

private:
   std::stack<GameObject*> m_Stack;
};

GameObject& DeSerializeHierarchy( std::string path );

void SerializeHierarchy( GameObject& root, std::string path );

GameObject& HierarchyForNewProject( std::vector<GameObject*>& gameObjects );

GameObject& CreateLightGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent );

GameObject& CreateCameraGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent );

GameObject& CreateCubeGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent );

void CopyStringToBuffer( char *buffer, std::string str );
