#include "PrefabField.h"
#include "World.h"
#include "GameObject.h"

GameObject& PrefabField::Instantiate( const World& world ) const
{
   // Use Value() to load profab GO from asset loader, clone it, then awake it.
   throw std::exception("no imp");
}