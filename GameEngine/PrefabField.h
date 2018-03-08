#pragma once
#include <vector>
#include "SerializedFields.h"

class World;
class GameObject;
class Editor;
class Transform;

class PrefabField : public SerializedString
{
public:
   GameObject& Instantiate( World& world, Transform& parent );
   GameObject& InstantiateEdit( Editor& editor, Transform& parent );

private:
   GameObject& CreateInstance( Transform& parent, World& world, std::vector<GameObject*>& hierarchy );
};