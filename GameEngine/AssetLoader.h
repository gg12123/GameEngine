#pragma once
#include <unordered_map>
#include "Asset.h"

typedef Asset*( *AssetCreationFunctionPtr )();

class AssetLoader
{
public:

   ~AssetLoader();

   // Name must be with extension.
   Asset& LoadIfNotAlreadyLoaded( std::string name, AssetCreationFunctionPtr creator );

private:
   std::unordered_map<std::string, Asset*> m_Assets;
};
