#include "Transform.h"
#include "Debug.h"

std::list<Transform*>::iterator Transform::ChildrenBegin()
{
   return m_Childern.begin;
}

std::list<Transform*>::iterator Transform::ChildrenEnd()
{
   return m_Childern.end;
}

std::list<Transform*>::iterator Transform::RegisterChild( Transform& child )
{
   m_Childern.push_back(&child);
   return m_Childern.back;
}

void Transform::UnRegisterChild( std::list<Transform*>::iterator toChild )
{
   m_Childern.erase( toChild );
}

void Transform::SetParent( Transform& parent )
{
   m_Parent->UnRegisterChild( m_ToThisInParentsChildList );
   m_Parent = &parent;
   m_ToThisInParentsChildList = m_Parent->RegisterChild( *this );
}

void Transform::ClearParent()
{
   SetParent( *m_Root );
}

void Transform::InitParent( Transform& parent )
{
   m_Parent = &parent;
   m_ToThisInParentsChildList = m_Parent->RegisterChild( *this );
}

Matrix4 Transform::GetLocalTransformMatrix()
{
   // calculate it
}

Matrix4 Transform::GetGlobalTransformMatrix()
{
   if (m_Dirty)
   {
      Debug::Instance().LogError( "Attempt to get global matrix when dirty" );
   }

   return m_TransformMatrix;
}