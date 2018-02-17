#pragma once
#include <stack>
#include <string>
#include "GameObject.h"

class EnumerableHierarchy
{
public:
   EnumerableHierarchy( GameObject& const root );

   GameObject* Next();

private:
   std::stack<GameObject*> m_Stack;
};

GameObject& DeSerializeHierarchy( std::string path );

void SerializeHierarchy( GameObject& root, std::string path );
