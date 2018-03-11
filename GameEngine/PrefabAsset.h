#pragma once
#include <vector>
#include "Asset.h"

class PrefabAsset : public Asset
{
public:
   PrefabAsset();

   void Load( std::string name ) override;
   PrefabAsset& PrefabValue() override;
   static Asset* CreateInstance();

   GameObject& GetRoot();
   std::vector<GameObject*>& GetHierarchy();

private:
   GameObject* m_Root;
   std::vector<GameObject*> m_Hierarchy;
};
