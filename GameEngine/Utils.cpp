#include <unordered_map>
#include "Utils.h"
#include "Component.h"

EnumerableHierarchy::EnumerableHierarchy( GameObject& const root )
{
   m_Stack.push( &root );
}

GameObject* EnumerableHierarchy::Next()
{
   GameObject* next = nullptr;

   if (m_Stack.size() > 0)
   {
      next = m_Stack.top();
      m_Stack.pop();

      for (std::list<Transform*>::iterator it = next->GetTransfrom().ChildrenBegin(); it != next->GetTransfrom().ChildrenEnd(); it++)
      {
         m_Stack.push( &((*it)->GetGameObject()) );
      }
   }

   return next;
}

void SerializeHierarchy( GameObject& root, std::string path )
{
   std::ofstream stream;
   std::unordered_map<GameObject*, int32_t> gameObjToID;
   SerializedInt32 id;

   EnumerableHierarchy enumerator( root );
   int32_t currID = 0;
   GameObject* next = enumerator.Next();

   stream.open( path, std::ios::binary );

   if (!stream.is_open())
   {
      throw std::exception( "Unable to open file" );
   }

   // reserve space for object count
   stream.seekp( sizeof( int32_t ), stream.beg );

   // serialize the root

   // id
   id.SetValue( currID );
   id.Serialize( stream );
   gameObjToID[ next ] = currID;
   currID++;

   // components
   next->Serialize( stream );

   // do the rest
   next = enumerator.Next();

   while (next != nullptr)
   {
      // id
      id.SetValue( currID );
      id.Serialize( stream );
      gameObjToID[ next ] = currID;
      currID++;

      // parent
      id.SetValue( gameObjToID[ &next->GetTransfrom().GetParent().GetGameObject() ] );
      id.Serialize( stream );

      // components
      next->Serialize( stream );

      next = enumerator.Next();
   }

   // object count
   stream.seekp( 0, stream.beg );
   id.SetValue( currID ); // currID is now equal to count
   id.Serialize( stream );
}

GameObject& DeSerializeHierarchy( std::string path )
{
   std::unordered_map<int32_t, GameObject*> idToGameObj;
   SerializedInt32 id;
   std::ifstream stream;

   stream.open( path, std::ios::binary );

   if (!stream.is_open())
   {
      throw std::exception( "Unable to open file" );
   }

   // Get the number of objects
   id.DeSerialize( stream );
   int32_t numObjects = id.Value();

   // Deserialize the root
   GameObject *root = new GameObject();

   // get the id
   id.DeSerialize( stream );
   idToGameObj[ id.Value ] = root;

   // there is no parent id for the root
   
   // add components
   root->DeSerialize( stream );

   // The other game objs
   for (int32_t i = 1; i < numObjects; i++)
   {
      GameObject *obj = new GameObject();

      // get the id
      id.DeSerialize( stream );
      idToGameObj[ id.Value ] = obj;

      // get the parent id
      id.DeSerialize( stream );

      // add components
      obj->DeSerialize( stream );

      // init parent
      obj->GetTransfrom().InitParent( (idToGameObj[ id.Value ])->GetTransform() );
   }

   return *root;
}
