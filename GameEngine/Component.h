#pragma once
#include <unordered_map>
#include <string>
#include "Functions.h"
#include "SerializedFields.h"
#include "PrefabField.h"
#include "MeshField.h"
#include "ISearializedFieldOwner.h"
#include "IInput.h"

class GameObject;
class World;
class Transform;
class IEditor;
class Editor;
class Camera;

class Component : public ISerializedFieldOwner
{
public:
   Component();
   virtual ~Component();

   void PreAwake( GameObject& gameObject );
   void Awake( World &world );
   void EditAwake( Editor &editor );

   virtual void Start();

   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void DeSerialize( std::ifstream& stream ) override;
   void Serialize( std::ofstream& stream ) override;
   std::string GetName() override;
   void OnNewSerializedFields() override;

   GameObject& GetGameObject();
   Transform& GetTransform();

   virtual int32_t GetType() = 0;

   virtual void Update();
   virtual void FixedUpdate();
   virtual void EditUpdate();

   Component& Clone();

protected:
   virtual void SetWorld( World &world ) = 0;
   virtual void Awake();
   virtual void EditAwake( IEditor& editor );

   void RegisterForUpdate( const EUpdaterFunction eFunction );

   Camera& GetActiveCamera();
   IInput& GetInput();

   void LoadScene( std::string name );

   GameObject& InstantiatePrefab( PrefabField& prefab );
   std::string InstantiateMesh( MeshField& mesh );

private:

   World *m_World;
   GameObject *m_GameObject;
};