#include <unordered_map>
#include <Windows.h>
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

TwoDimentionalSpace::TwoDimentionalSpace( const float halfWidth, const float halfHeight, const vmath::vec2 centre )
{
   m_HalfWidth = halfWidth;
   m_HalfHeight = halfHeight;
   m_Centre = centre;
}

float TwoDimentionalSpace::HalfWidth() const
{
   return m_HalfWidth;
}

float TwoDimentionalSpace::HalthHeight() const
{
   return m_HalfHeight;
}

vmath::vec2 TwoDimentionalSpace::Centre() const
{
   return m_Centre;
}

void SerializeHierarchy( GameObject& root, std::ofstream& stream )
{
   std::unordered_map<GameObject*, int32_t> gameObjToID;
   SerializedInt32 id;
   SerializedString name;

   EnumerableHierarchy enumerator( root );
   int32_t currID = 0;
   GameObject* next = enumerator.Next();

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

GameObject& DeSerializeHierarchy( std::ifstream& stream, std::vector<GameObject*>& gameObjects )
{
   std::unordered_map<int32_t, GameObject*> idToGameObj;
   SerializedInt32 id;

   // Get the number of objects
   id.DeSerialize( stream );
   int32_t numObjects = id.Value();

   // Deserialize the root
   GameObject *root = new GameObject();
   gameObjects.push_back( root );

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
      gameObjects.push_back( obj );

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

   cube->AddComponent( *(ComponentCreator::Instance().Create( COMPONENT_ID_DIFFUSEMESHRENDERER )) );

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
   gameObjects.push_back( &CreateLightGameObject( vmath::vec3( 0.0f, 0.0f, 10.0f ), vmath::eulerToMatrix( 180.0f, 45.0f, 0.0f ), root->GetTransform() ) );

   return *root;
}

void CopyStringToBuffer( char *buffer, const std::string str )
{
   for (unsigned int i = 0; i < str.size(); i++)
      buffer[ i ] = str.at( i );

   buffer[ str.size() ] = '\0';
}

void GetFileNamesInDirectory( const std::string directory, std::vector<std::string>& out )
{
   HANDLE dir;
   WIN32_FIND_DATA file_data;

   if ((dir = FindFirstFile( (directory + "/*").c_str(), &file_data )) == INVALID_HANDLE_VALUE)
      return; /* No files found */

   do {
      const std::string file_name = file_data.cFileName;
      const std::string full_file_name = directory + "/" + file_name;
      const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

      if (file_name[ 0 ] == '.')
         continue;

      if (is_directory)
         continue;

      out.push_back( file_name );
   } while (FindNextFile( dir, &file_data ));

   FindClose( dir );
}

static unsigned int FindIndexOfParent( const std::vector<GameObject*>& objects, GameObject* obj )
{
   Transform* parent = &obj->GetTransform().GetParent();
   unsigned int index = -1;

   for (unsigned int i = 0; i < objects.size(); i++)
   {
      if (&objects.at( i )->GetTransform() == parent)
      {
         index = i;
         break;
      }
   }

   if (index < 0)
   {
      throw std::exception( "Unable to find parent during GO duplication" );
   }

   return index;
}

GameObject& DuplicateHierarchy( GameObject& root, std::vector<GameObject*>& duplicatedObjs )
{
   std::vector<GameObject*> gameObjectsToDup;

   EnumerableHierarchy enumerator( root );
   GameObject* next = enumerator.Next();

   while (next)
   {
      gameObjectsToDup.push_back( next );
      next = enumerator.Next();
   }

   return DuplicateHierarchy( root, gameObjectsToDup, duplicatedObjs );
}

// Note - this is used for prefab instantiation so it must not assume that the game objects to
// duplicate have been awoken.
GameObject& DuplicateHierarchy( GameObject& root, const std::vector<GameObject*>& gameObjects, std::vector<GameObject*>& duplicatedObjs )
{
   // Clone all the objects
   for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
   {
      duplicatedObjs.push_back( &(*it)->Clone() );
   }

   // Set up the parent child relationship
   GameObject* duplicatesRoot = nullptr;

   for (unsigned int i = 0; i < gameObjects.size(); i++)
   {
      GameObject* obj = gameObjects.at( i );

      if (obj != &root)
      {
         unsigned int indexOfParent = FindIndexOfParent( gameObjects, obj );
         duplicatedObjs.at( i )->GetTransform().InitParent( duplicatedObjs.at( indexOfParent )->GetTransform() );
      }
      else
      {
         duplicatesRoot = duplicatedObjs.at( i );
      }
   }

   if (!duplicatesRoot)
   {
      throw std::exception( "failed to find root during duplication" );
   }

   return *duplicatesRoot;
}

vmath::vec2 Transform2DPoint( const TwoDimentionalSpace& sourceSpace,
                              const TwoDimentionalSpace& destSpace,
                              const vmath::vec2& sourcePoint )
{
   vmath::vec2 x = Transform2DDirection( sourceSpace, destSpace, sourcePoint - sourceSpace.Centre() );

   return (destSpace.Centre() + x);
}

vmath::vec2 Transform2DDirection( const TwoDimentionalSpace& sourceSpace,
                                  const TwoDimentionalSpace& destSpace,
                                  const vmath::vec2& sourceDir )
{
   const float pW = sourceDir[ 0 ] / sourceSpace.HalfWidth();
   const float pH = sourceDir[ 1 ] / sourceSpace.HalfWidth();

   return vmath::vec2( pW * destSpace.HalfWidth(), pH * destSpace.HalthHeight() );
}
