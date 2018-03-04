#pragma once
#include <unordered_map>
#include <vector>
#include "Functions.h"
#include "Component.h"
#include "SerializedFields.h"

class World;
class Transform;

class GameObject
{
public:

   ~GameObject();

   GameObject();
   GameObject( std::string name );

   std::string GetName();
   void SetName( std::string name );

   // called by the world if this object starts in the scene. If the object is instantiated, awake
   // must be called by the instantitor.
   void AwakeComponents( World &world );

   // called by the world before the next update
   void StartComponents();

   // called by the world
   void Update();
   void FixedUpdate();
   void EditUpdate();

   void AddComponent( Component &component );
   void RegisterComponentForUpdate( const EUpdaterFunction updateFunction, Component& comp );

   Transform& GetTransform();

   void DeSerialize( std::ifstream& stream );
   void Serialize( std::ofstream& stream );

   // do this inside DeSerialize()
   void CacheTransform();

   template <class T>
   T* GetComponent();

   std::vector<Component*>::iterator ComponentsBegin();
   std::vector<Component*>::iterator ComponentsEnd();

private:

   void CommonConstructor();

   std::vector<Component*>* m_UpdateableComponents[ NUMBER_OF_UPDATE_FUNCTIONS ];
   std::vector<Component*> m_Components;

   World *m_World;
   Transform *m_Transform;

   std::string m_Name;
};

template <class T>
T* GameObject::GetComponent()
{
   T* comp = nullptr;

   for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
   {
      comp = dynamic_cast<T*>(*it);

      if (comp)
         break;
   }

   return comp;
}

