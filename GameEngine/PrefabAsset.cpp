#include <iostream>
#include <fstream>
#include "PrefabAsset.h"
#include "Utils.h"
#include "Path.h"


PrefabAsset::PrefabAsset()
{
   m_Root = nullptr;
}

void PrefabAsset::Load( std::string name )
{
   std::string path = Path::Instance().GetPrefabPath( name );

   std::ifstream stream;
   stream.open( path, std::ios::binary );

   if (!stream.is_open())
   {
      throw std::exception( "Unable to open prefab file." );
   }

   m_Hierarchy.clear(); // should be clear anyway
   m_Root = &DeSerializeHierarchy( stream, m_Hierarchy );
}

PrefabAsset* PrefabAsset::PrefabValue()
{
   return this;
}

Asset* PrefabAsset::CreateInstance()
{
   return new PrefabAsset();
}

GameObject& PrefabAsset::GetRoot()
{
   return *m_Root;
}

std::vector<GameObject*>& PrefabAsset::GetHierarchy()
{
   return m_Hierarchy;
}