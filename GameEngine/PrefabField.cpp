#include "PrefabField.h"
#include "World.h"
#include "GameObject.h"
#include "PrefabAsset.h"
#include "Editor.h"
#include "Utils.h"
#include "Transform.h"

static unsigned int FindIndexOfParent( std::vector<GameObject*>& objects, GameObject* obj )
{
   Transform* parent = &obj->GetTransform().GetParent();
   unsigned int index = -1;

   for (unsigned int i = 0; i < objects.size(); i++)
   {
      if (&objects.at( i )->GetTransform() == parent)
      {
         index = i;
         break;
      }
   }

   if (index < 0)
   {
      throw std::exception( "Unable to find parent during prefab instantaion" );
   }

   return index;
}

GameObject& PrefabField::Instantiate( World& world, Transform& parent )
{
   std::vector<GameObject*> instanceHierarchy;
   CreateInstance( parent, world, instanceHierarchy );

   for (auto it = instanceHierarchy.begin(); it != instanceHierarchy.end(); it++)
   {
      (*it)->AwakeComponents( world );
   }
}

GameObject& PrefabField::InstantiateEdit( Editor& editor, Transform& parent )
{
   std::vector<GameObject*> instanceHierarchy;
   CreateInstance( parent, editor.GetWorld(), instanceHierarchy );

   for (auto it = instanceHierarchy.begin(); it != instanceHierarchy.end(); it++)
   {
      (*it)->EditAwakeComponents( editor.GetWorld() );
      // pass editor to special components
   }
}

void PrefabField::CreateInstance( Transform& parent, World& world, std::vector<GameObject*>& instanceHierarchy )
{
   PrefabAsset* prefabAsset = world.GetAssetLoader().LoadIfNotAlreadyLoaded( Value(), PrefabAsset::CreateInstance ).PrefabValue();

   GameObject* prefabRoot = &prefabAsset->GetRoot();
   std::vector<GameObject*>& prefabHierarchy = prefabAsset->GetHierarchy();

   // Clone all the objects
   for (auto it = prefabHierarchy.begin(); it != prefabHierarchy.end(); it++)
   {
      instanceHierarchy.push_back( &(*it)->Clone() );
   }

   // Set up the parent child relationship
   GameObject* instanceRoot = nullptr;

   for (unsigned int i = 0; i < prefabHierarchy.size(); i++)
   {
      GameObject* obj = prefabHierarchy.at( i );

      if (obj != prefabRoot)
      {
         unsigned int indexOfParent = FindIndexOfParent( prefabHierarchy, obj );
         instanceHierarchy.at( i )->GetTransform().InitParent( instanceHierarchy.at( indexOfParent )->GetTransform() );
      }
      else
      {
         instanceRoot = instanceHierarchy.at( i );
      }
   }

   if (!instanceRoot)
   {
      throw std::exception( "failed to find root of prefab" );
   }

   instanceRoot->GetTransform().InitParent( parent );
}