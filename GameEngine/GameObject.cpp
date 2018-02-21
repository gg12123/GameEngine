#include "GameObject.h"
#include "ComponentCreator.h"

GameObject::GameObject()
{
   m_Transform = nullptr;
}

Transform& GameObject::GetTransfrom()
{
   return *m_Transform;
}

void GameObject::UpdateComponents( EUpdaterFunction updateFunction )
{
   std::vector<UpdaterFunctionPtr> *functions = &m_UpdaterFunctions[ updateFunction ];

   for (std::vector<UpdaterFunctionPtr>::iterator it = functions->begin; it != functions->end; it++)
   {
      (*it)();
   }
}

void GameObject::AwakeComponents( World &world )
{
   m_World = &world;

   for (std::vector<Component*>::iterator it = m_Components.begin; it != m_Components.end; it++)
   {
      (*it)->Awake( world, *this );
   }

   // this will make the world call start
   m_World->RegisterForStart( *this );
}

void GameObject::StartComponents()
{
   for (std::vector<Component*>::iterator it = m_Components.begin; it != m_Components.end; it++)
   {
      (*it)->Start();
   }
}

void GameObject::AddComponent( Component &component )
{
   m_Components.push_back( &component );
}

void GameObject::RegisterUpdaterFunction( EUpdaterFunction updateFunc, UpdaterFunctionPtr updaterPtr )
{
   if (m_UpdaterFunctions[ updateFunc ].size == 0)
   {
      m_World->RegisterToUpdateFunction( updateFunc, *this );
   }

   m_UpdaterFunctions[ updateFunc ].push_back( updaterPtr );
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