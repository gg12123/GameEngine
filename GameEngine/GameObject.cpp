#include "GameObject.h"
#include "ComponentCreator.h"
#include "World.h"
#include "Transform.h"
#include "Debug.h"
#include "Editor.h"

std::vector<Component*>* UpdateableComponents::Components()
{
   return &m_Components;
}

NullableValue<std::list<GameObject*>::iterator>& UpdateableComponents::ToThisInWorld()
{
   return m_ToMyGOInWorldsUpdateList;
}

GameObject::~GameObject()
{
   for (auto it = m_Components.begin(); it != m_Components.end(); it++)
   {
      delete *it;
   }

   for (int i = 0; i < NUMBER_OF_UPDATE_FUNCTIONS; i++)
   {
      if (m_UpdateableComponents[ i ])
      {
         delete m_UpdateableComponents[ i ];
      }
   }

   m_Transform = nullptr;
   m_World = nullptr;
}

void GameObject::OnDestroy()
{
   // un-register this go from the world
   for (int i = 0; i < NUMBER_OF_UPDATE_FUNCTIONS; i++)
   {
      UpdateableComponents* x = m_UpdateableComponents[ i ];

      if (x && !x->ToThisInWorld().IsNull())
      {
         m_World->UnRegisterToUpdateFunction( (EUpdaterFunction)i, x->ToThisInWorld().Get() );
         x->ToThisInWorld().Clear(); // not actually nessary here but will be when going inactive
      }
   }

   // invoke the event on components
   InvokeEvent( eOnDestroy );
}

GameObject::GameObject()
{
   CommonConstructor();
   m_Name = "GameObject";
}

GameObject::GameObject( std::string name )
{
   CommonConstructor();
   m_Name = name;
}

std::string GameObject::GetName()
{
   return m_Name;
}

void GameObject::SetName( std::string name )
{
   m_Name = name;
}

void GameObject::CommonConstructor()
{
   m_Transform = nullptr;

   for (int i = 0; i < NUMBER_OF_UPDATE_FUNCTIONS; i++)
   {
      m_UpdateableComponents[ i ] = nullptr;
   }
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
      for (auto it = v->Components()->begin(); it != v->Components()->end(); it++)
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
      for (auto it = v->Components()->begin(); it != v->Components()->end(); it++)
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
      for (auto it = v->Components()->begin(); it != v->Components()->end(); it++)
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

void GameObject::EditAwakeComponents( Editor &editor )
{
   m_World = &editor.GetWorld();

   for (std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); it++)
   {
      (*it)->EditAwake( editor, *this );
   }
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
      m_UpdateableComponents[ updateFunction ] = new UpdateableComponents();
      m_UpdateableComponents[ updateFunction ]->ToThisInWorld().Set( m_World->RegisterToUpdateFunction( updateFunction, *this ) );
   }

   std::vector<Component*>* v = m_UpdateableComponents[ updateFunction ]->Components();

   if (std::find( v->begin(), v->end(), &comp ) != v->end())
   {
      Debug::Instance().LogError( "Component registers twice for update" );
   }

   v->push_back( &comp );
}

void GameObject::InvokeEvent( EGameObjectEvent eventID )
{
   m_Events.Invoke( eventID );
}

void GameObject::RegisterForEvent( EGameObjectEvent eventID, EventHandler& handler )
{
   m_Events.Register( eventID, handler );
}

void GameObject::CacheTransform()
{
   m_Transform = GetComponent<Transform>();
}

void GameObject::Serialize( std::ofstream& stream )
{
   SerializedInt32 number;
   SerializedString name;

   name.SetValue( GetName() );
   name.Serialize( stream );

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
   SerializedString name;

   name.DeSerialize( stream );
   SetName( name.Value() );

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

GameObject& GameObject::Clone()
{
   GameObject* clone = new GameObject( GetName() );

   for (auto it = m_Components.begin(); it != m_Components.end(); it++)
   {
      clone->AddComponent( (*it)->Clone() );
   }

   clone->CacheTransform();

   return *clone;
}

bool GameObject::IsPartOfHierarchy( GameObject& root ) const
{
   Transform* t = m_Transform;
   Transform* rootsTransform = &root.GetTransform();
   Transform* worldsRoot = &m_World->GetRootTransform();

   while ((t != worldsRoot) && (t != rootsTransform))
   {
      t = &t->GetParent();
   }

   return (t == rootsTransform);
}