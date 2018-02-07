#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "SerializedFields.h"
#include "Matrix4.h"

class Transform : public Component
{
public:

   Vector3 GetLocalPosition();
   Vector3 GetPosition();
   Quaternion GetRotation();
   Quaternion GetLocalRotation();

   Vector3 SetLocalPosition( Vector3 pos );
   Vector3 SetPosition( Vector3 pos );
   Quaternion SetRotation( Quaternion rot );
   Quaternion SetLocalRotation( Quaternion rot );

   Matrix4 GetLocalTransformMatrix();
   Matrix4 GetGlobalTransformMatrix();

   void SetParent( Transform& parent );
   void InitParent( Transform& parent );
   void ClearParent();

   bool IsDirty();

   // allow outside code to iterate over the children but not change the list
   std::list<Transform*>::iterator ChildrenBegin();
   std::list<Transform*>::iterator ChildrenEnd();

   std::list<Transform*>::iterator RegisterChild( Transform& child );
   void UnRegisterChild( std::list<Transform*>::iterator toChild );

protected:

   void Awake();

private:

   bool m_Dirty;

   // invalid when dirty
   Vector3 m_Position;
   Quaternion m_Rotation;
   Matrix4 m_TransformMatrix;

   // always valid
   SerializedVector3 m_LocalVector3;
   SerializedQuaternion m_LocalRotation;

   Transform *m_Parent;
   Transform *m_Root;
   std::list<Transform*>::iterator m_ToThisInParentsChildList;

   std::list<Transform*> m_Childern;
};
