#pragma once
#include "MyVmath.h"
#include "Component.h"
#include "Quaternion.h"
#include "SerializedFields.h"

using namespace vmath;

class Transform : public Component
{
public:

   Transform();

   vec3 GetLocalPosition();
   vec3 GetPosition();
   vec3 GetPositionUnconditional(); // used when you know that the hierarchy above is clean

   Quaternion GetRotation();
   Quaternion GetLocalRotation();
   Quaternion GetRotationUnconditional(); // used when you know that the hierarchy above is clean

   vec3 SetLocalPosition( vec3 pos );
   vec3 SetPosition( vec3 pos );
   Quaternion SetRotation( Quaternion rot );
   Quaternion SetLocalRotation( Quaternion rot );

   mat4 GetTransformMatrix();

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

protected:

   void Awake();

private:

   void SetDirty();
   Transform& FindCleanHierarchy();

   bool m_Dirty;

   // invalid when dirty
   vec3 m_Position;
   Quaternion m_Rotation;
   mat4 m_TransformMatrix;

   // always valid
   SerializedVector3 m_LocalPosition;
   SerializedQuaternion m_LocalRotation;

   Transform *m_Parent;
   Transform *m_Root;
   std::list<Transform*>::iterator m_ToThisInParentsChildList;

   std::list<Transform*> m_Childern;

   TransformUpdater *m_TransformUpdater;
};
