#pragma once
#include "MyVmath.h"
#include "Component.h"
#include "SerializedFields.h"

using namespace vmath;

class Transform : public Component
{
public:
   Transform();

   vec3 GetLocalPosition();
   vec3 GetPosition();

   mat4 GetRotation();
   mat4 GetLocalRotation();

   void SetLocalPosition( vec3 pos );
   void SetLocalRotation( mat4 rot );

   mat4 GetTransformMatrixAssumingClean();
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

protected:

   void Awake();

private:

   void SetDirty();
   Transform& FindCleanHierarchy();
   void ConstructLocalTransformMatrix();
   void ConstructTransformMatrix();

   bool m_Dirty;

   // invalid when dirty
   mat4 m_TransformMatrix;

   // always valid
   SerializedVector3 m_LocalPosition;
   SerializedRotation m_LocalRotation;
   mat4 m_LocalTransformMatrix;

   Transform *m_Parent;
   Transform *m_Root;
   std::list<Transform*>::iterator m_ToThisInParentsChildList;

   std::list<Transform*> m_Childern;

   TransformUpdater *m_TransformUpdater;
};
