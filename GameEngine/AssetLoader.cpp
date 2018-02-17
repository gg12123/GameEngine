#include "AssetLoader.h"

Asset& AssetLoader::LoadIfNotAlreadyLoaded( std::string name, AssetCreationFunctionPtr creator )
{
   Asset* out = nullptr;
   std::unordered_map<std::string, Asset*>::iterator it = m_Assets.find( name );

   if (it != m_Assets.end())
   {
      out = it->second;
   }
   else
   {
      out = creator();
      out->Load( name );
      m_Assets[ name ] = out;
   }

   return *out;
}