#include "GameObject.h"
#include "ComponentCreator.h"
#include "World.h"
#include "Transform.h"
#include "Debug.h"

GameObject::GameObject()
{
   m_Transform = nullptr;
   m_UpdateableComponents[ eUpdateFunction ] = nullptr;
   m_UpdateableComponents[ eFixedUpdateFunction ] = nullptr;
   m_UpdateableComponents[ eUpdateInEditMode ] = nullptr;
}

Transform& GameObject::GetTransform()
{
   return *m_Transform;
}

void GameObject::Update()
{
   auto v = m_UpdateableComponents[ eUpdateFunction ];

   if (!v)
   {
      Debug::Instance().LogError( "Update called with no updatable comps" );
   }
   else
   {
      for (auto it = v->begin(); it != v->end(); it++)
      {
         (*it)->Update();
      }
   }
}

void GameObject::FixedUpdate()
{
   auto v = m_UpdateableComponents[ eFixedUpdateFunction ];

   if (!v)
   {
      Debug::Instance().LogError( "Fixed update called with no updatable comps" );
   }
   else
   {
      for (auto it = v->begin(); it != v->end(); it++)
      {
         (*it)->FixedUpdate();
      }
   }
}

void GameObject::EditUpdate()
{
   auto v = m_UpdateableComponents[ eUpdateInEditMode ];

   if (!v)
   {
      Debug::Instance().LogError( "Edit update called with no updatable comps" );
   }
   else
   {
      for (auto it = v->begin(); it != v->end(); it++)
      {
         (*it)->EditUpdate();
      }
   }
}

void GameObject::AwakeComponents( World &world )
{
   m_World = &world;

   for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
   {
      (*it)->Awake( world, *this );
   }

   // this will make the world call start
   m_World->RegisterForStart( *this );
}

void GameObject::StartComponents()
{
   for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
   {
      (*it)->Start();
   }
}

void GameObject::AddComponent( Component &component )
{
   m_Components.push_back( &component );
}

void GameObject::RegisterComponentForUpdate( const EUpdaterFunction updateFunction, Component& comp )
{
   if (!m_UpdateableComponents[ updateFunction ])
   {
      m_UpdateableComponents[ updateFunction ] = new std::vector<Component*>();
      m_World->RegisterToUpdateFunction( updateFunction, *this );
   }

   std::vector<Component*>* v = m_UpdateableComponents[ updateFunction ];

   if (std::find( v->begin(), v->end(), &comp ) != v->end())
   {
      Debug::Instance().LogError( "Component registers twice for update" );
   }

   v->push_back( &comp );
}

void GameObject::CacheTransform()
{
   m_Transform = GetComponent<Transform>();
}

void GameObject::Serialize( std::ofstream& stream )
{
   SerializedInt32 number;

   number.SetValue( m_Components.size() );
   number.Serialize( stream );

   for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
   {
      number.SetValue( (*it)->GetType() );
      number.Serialize( stream );

      (*it)->Serialize( stream );
   }
}

void GameObject::DeSerialize( std::ifstream& stream )
{
   SerializedInt32 number;
   number.DeSerialize( stream );
   int32_t count = number.Value();

   for (int i = 0; i < count; i++)
   {
      number.DeSerialize( stream );

      Component* comp = ComponentCreator::Instance().Create( number.Value() );
      comp->DeSerialize( stream );

      AddComponent( *comp );
   }

   CacheTransform();
}

std::vector<Component*>::iterator GameObject::ComponentsBegin()
{
   return m_Components.begin();
}

std::vector<Component*>::iterator GameObject::ComponentsEnd()
{
   return m_Components.end();
}