#include "PrefabField.h"
#include "World.h"
#include "GameObject.h"
#include "PrefabAsset.h"
#include "Editor.h"
#include "Utils.h"
#include "Transform.h"
#include "Path.h"

std::string PrefabField::GetPathToSelectables()
{
   return Path::Instance().GetPrefabPath( "" );
}

GameObject& PrefabField::Instantiate( World& world, Transform& parent )
{
   std::vector<GameObject*> instanceHierarchy;
   GameObject& root = CreateInstance( parent, world, instanceHierarchy );

   world.AwakeHierarchy( instanceHierarchy );

   return root;
}

GameObject& PrefabField::InstantiateEdit( Editor& editor, Transform& parent )
{
   std::vector<GameObject*> instanceHierarchy;
   GameObject& root = CreateInstance( parent, editor.GetWorld(), instanceHierarchy );

   editor.GetWorld().EditAwakeHierarchy( editor, instanceHierarchy );

   return root;
}

GameObject& PrefabField::CreateInstance( Transform& parent, World& world, std::vector<GameObject*>& instanceHierarchy )
{
   PrefabAsset& prefabAsset = world.GetAssetLoader().LoadIfNotAlreadyLoaded( Value(), PrefabAsset::CreateInstance ).PrefabValue();

   GameObject& prefabRoot = prefabAsset.GetRoot();
   std::vector<GameObject*>& prefabHierarchy = prefabAsset.GetHierarchy();

   GameObject& instanceRoot = DuplicateHierarchy( prefabRoot, prefabHierarchy, instanceHierarchy );

   instanceRoot.GetTransform().InitParent( parent );

   return instanceRoot;
}