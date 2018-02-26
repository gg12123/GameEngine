#include "GameObject.h"
#include "ComponentCreator.h"
#include "World.h"
#include "Transform.h"

GameObject::GameObject()
{
   m_Transform = nullptr;
}

Transform& GameObject::GetTransform()
{
   return *m_Transform;
}

void GameObject::UpdateComponents( EUpdaterFunction updateFunction )
{
   // If this update is being called by the world, it is garunteed that
   // the corresponding vector exists in the map. (becasue of the registration process)
   std::vector<UpdaterFunctionPtr> *functions = m_UpdaterFunctions[ updateFunction ];

   for (std::vector<UpdaterFunctionPtr>::iterator it = functions->begin(); it != functions->end(); it++)
   {
      (*it)();
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

void GameObject::RegisterUpdaterFunction( const EUpdaterFunction updateFunc, const UpdaterFunctionPtr updaterPtr )
{
   std::unordered_map<EUpdaterFunction, std::vector<UpdaterFunctionPtr>*>::iterator it;
   std::vector<UpdaterFunctionPtr>* v;

   it = m_UpdaterFunctions.find( updateFunc );

   if (it == m_UpdaterFunctions.end())
   {
      v = new std::vector<UpdaterFunctionPtr>();
      m_UpdaterFunctions[ updateFunc ] = v;

      m_World->RegisterToUpdateFunction( updateFunc, *this );
   }
   else
   {
      v = it->second;
   }

   v->push_back( updaterPtr );
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