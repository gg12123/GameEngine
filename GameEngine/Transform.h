#pragma once
#include "MyVmath.h"
#include "EngineComponent.h"
#include "SerializedFields.h"
#include "Events.h"
#include "GameObjectEvents.h"

using namespace vmath;

class Transform : public EngineComponent
{
public:
   Transform();

   vec3 ToLocalPosition( const vec3& globalPos );
   vec3 ToLocalDirection( const vec3& globalDir );
   vec3 ToGlobalDirection( const vec3& localDir );
   vec3 ToGlobalPosition( const vec3& localPos );

   vec3 GetLocalPosition();
   vec3 GetPosition();
   mat4 GetLocalRotation();
   mat4 GetRotation();
   vec3 GetLocalScale();
   vec3 GetScale();

   vec3 Forward();
   vec3 Right();
   vec3 Up();

   void SetLocalPosition( vec3 pos );
   void SetPosition( vec3 pos );
   void SetLocalRotation( mat4 rot );
   void SetRotation( mat4 rot );
   void SetLocalScale( vec3 s );
   void SetScale( vec3 s );

   mat4 GetTransformMatrixAssumingClean();
   mat4 GetTransformMatrix();
   mat4 GetLocalTransformMatrix();

   void SetParent( Transform& parent );
   void InitParent( Transform& parent );
   void ClearParent();
   Transform& GetParent();

   bool IsDirty();

   void Clean();

   // allow outside code to iterate over the children but not change the list
   std::list<Transform*>::iterator ChildrenBegin();
   std::list<Transform*>::iterator ChildrenEnd();

   std::list<Transform*>::iterator RegisterChild( Transform& child );
   void UnRegisterChild( std::list<Transform*>::iterator toChild );

   int32_t GetType() override;
   void GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields ) override;
   void OnNewSerializedFields() override;

   void OnDestroy();

protected:

   void Awake() override;
   void EditAwake( IEditor& editor ) override;

private:

   vec3 InternalToLocalDirection( const vec3& globalDir );
   vec3 InternalToGlobalDirection( const vec3& localDir );

   void SetDirty();
   Transform& FindCleanHierarchy();
   void ConstructLocalTransformMatrix();
   void ConstructTransformMatrix();

   bool m_Dirty;

   // invalid when dirty
   mat4 m_TransformMatrix;

   // always valid
   SerializedVector3 m_LocalPosition;
   SerializedVector3 m_LocalScale;
   SerializedRotation m_LocalRotation;
   mat4 m_LocalTransformMatrix;

   Transform *m_Parent;
   Transform *m_Root;
   std::list<Transform*>::iterator m_ToThisInParentsChildList;

   std::list<Transform*> m_Childern;

   TransformUpdater *m_TransformUpdater;

   VoidEventHandler<Transform> m_OnDestroyEvent;
};
