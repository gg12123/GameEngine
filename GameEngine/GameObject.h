#pragma once
#include <unordered_map>
#include <vector>
#include "Functions.h"
#include "Component.h"
#include "SerializedFields.h"
#include "NullableIterator.h"
#include "EventManager.h"
#include "GameObjectEvents.h"
#include "Utils.h"

class World;
class Transform;
class Editor;

class UpdateableComponents
{
public:
   std::vector<Component*>* Components();
   NullableValue<std::list<GameObject*>::iterator>& ToThisInWorld();
private:
   std::vector<Component*> m_Components;
   NullableValue<std::list<GameObject*>::iterator> m_ToMyGOInWorldsUpdateList;
};

class GameObject
{
public:

   ~GameObject();

   GameObject();
   GameObject( std::string name );

   std::string GetName();
   void SetName( std::string name );

   void PreAwake();
   void AwakeComponents( World &world );
   void EditAwakeComponents( Editor &editor );

   // called by the world before the next update
   void StartComponents();

   // called by the world
   void Update();
   void FixedUpdate();
   void EditUpdate();

   void OnDestroy();

   void AddComponent( Component &component );
   void RegisterComponentForUpdate( const EUpdaterFunction updateFunction, Component& comp );

   Transform& GetTransform();

   void DeSerialize( std::ifstream& stream );
   void Serialize( std::ofstream& stream );

   // do this inside DeSerialize()
   void CacheTransform();

   void InvokeEvent( EGameObjectEvent eventID );
   void RegisterForEvent( EGameObjectEvent eventID, EventHandler& handler );

   template <class T>
   T* GetComponent();
   template<class T>
   void GetComponentsInChildren( std::vector<T*>& components );

   std::vector<Component*>::iterator ComponentsBegin();
   std::vector<Component*>::iterator ComponentsEnd();

   GameObject& Clone();

   bool IsPartOfHierarchy( GameObject& root ) const;

private:
   void CommonConstructor();

   UpdateableComponents* m_UpdateableComponents[ NUMBER_OF_UPDATE_FUNCTIONS ];
   std::vector<Component*> m_Components;
   EventManager<EGameObjectEvent, NUMBER_OF_GAME_OBJECT_EVENTS> m_Events;

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

template <class T>
void GameObject::GetComponentsInChildren( std::vector<T*>& components )
{
   EnumerableHierarchy enumerator( *this );

   GameObject* next = enumerator.Next();

   while (next)
   {
      T* comp = next->GetComponent<T>();

      if (comp)
         components.push_back( comp );

      next = enumerator.Next();
   }
}

