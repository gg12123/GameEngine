#include <unordered_map>
#include "Utils.h"
#include "Component.h"
#include "ComponentCreator.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "MeshRenderer.h"

EnumerableHierarchy::EnumerableHierarchy( GameObject& root )
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

      for (std::list<Transform*>::iterator it = next->GetTransform().ChildrenBegin(); it != next->GetTransform().ChildrenEnd(); it++)
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
      id.SetValue( gameObjToID[ &next->GetTransform().GetParent().GetGameObject() ] );
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
   idToGameObj[ id.Value() ] = root;

   // there is no parent id for the root
   
   // add components
   root->DeSerialize( stream );

   // The other game objs
   for (int32_t i = 1; i < numObjects; i++)
   {
      GameObject *obj = new GameObject();

      // get the id
      id.DeSerialize( stream );
      idToGameObj[ id.Value() ] = obj;

      // get the parent id
      id.DeSerialize( stream );

      // add components
      obj->DeSerialize( stream );

      // init parent
      obj->GetTransform().InitParent( (idToGameObj[ id.Value() ])->GetTransform() );
   }

   return *root;
}

static void InitTransformState( Transform& tran, vmath::vec3 pos, vmath::mat4 rot )
{
   std::unordered_map<std::string, SerializedField*> fields;
   tran.GetSerializedFields( fields );

   dynamic_cast<SerializedRotation*>(fields[ "rotation" ])->SetValue( rot );
   dynamic_cast<SerializedVector3*>(fields[ "position" ])->SetValue( pos );
   dynamic_cast<SerializedVector3*>(fields[ "scale" ])->SetValue( vec3( 1.0f, 1.0f, 1.0f ) );
}

GameObject& CreateCubeGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent )
{
   GameObject* cube = new GameObject( "Cube" );

   cube->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_TRANSFORM )) );
   cube->CacheTransform();
   cube->GetTransform().InitParent( parent );
   InitTransformState( cube->GetTransform(), pos, rot );

   cube->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_MESHRENDERER )) );

   std::unordered_map<std::string, SerializedField*> fields;
   cube->GetComponent<MeshRenderer>()->GetSerializedFields(fields);

   dynamic_cast<SerializedString*>(fields[ "meshName" ])->SetValue( "cube.mesh" );
   dynamic_cast<SerializedString*>(fields[ "shaderName" ])->SetValue( "diffuse.txt" );

   return *cube;
}

GameObject& CreateCameraGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent )
{
   GameObject* cam = new GameObject( "Camera" );

   cam->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_TRANSFORM )) );
   cam->CacheTransform();
   cam->GetTransform().InitParent( parent );
   InitTransformState( cam->GetTransform(), pos, rot );

   cam->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_CAMERA )) );
   Camera* camComp = cam->GetComponent<Camera>();
   camComp->SetFOV( 45.0f );
   camComp->SetNearClip( 0.3f );
   camComp->SetFarClip( 1000.0f );

   return *cam;
}

GameObject& CreateLightGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent )
{
   GameObject* light = new GameObject( "Light" );

   light->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_TRANSFORM )) );
   light->CacheTransform();
   light->GetTransform().InitParent( parent );
   InitTransformState( light->GetTransform(), pos, rot );

   light->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_LIGHT )) );

   return *light;
}

// In this function, the created components cannot interact with the world so be
// carefull what functions you call on them.
GameObject& HierarchyForNewProject( std::vector<GameObject*>& gameObjects )
{
   // root
   GameObject* root = new GameObject( "Root" );

   root->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_TRANSFORM )) );
   root->CacheTransform();
   InitTransformState( root->GetTransform(), vmath::vec3( 0.0f, 0.0f, 0.0f ), vmath::mat4().identity() );

   gameObjects.push_back( root );

   // cube
   gameObjects.push_back( &CreateCubeGameObject( vmath::vec3( 0.0f, 1.0f, 0.0f ), vmath::mat4().identity(), root->GetTransform() ) );

   // cube 2
   gameObjects.push_back( &CreateCubeGameObject( vmath::vec3( 0.0f, -1.0f, 0.0f ), vmath::mat4().identity(), root->GetTransform() ) );

   // camera
   gameObjects.push_back( &CreateCameraGameObject( vmath::vec3( 0.0f, 0.0f, 10.0f ), vmath::rotate( 0.0f, 180.0f, 0.0f ), root->GetTransform() ) );

   // light
   gameObjects.push_back( &CreateLightGameObject( vmath::vec3( 0.0f, 0.0f, 10.0f ), vmath::rotate( 0.0f, 180.0f, 0.0f ), root->GetTransform() ) );

   return *root;
}
