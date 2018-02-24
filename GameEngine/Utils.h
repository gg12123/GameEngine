#pragma once
#include <stack>
#include <string>

class GameObject;

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

GameObject& HierarchyForNewProject();
