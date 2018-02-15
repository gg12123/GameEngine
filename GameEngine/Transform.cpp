#include "Transform.h"
#include "Debug.h"

Transform::Transform()
{
   m_Parent = nullptr;
}

void Transform::Awake()
{
   m_TransformUpdater = &GetTransformUpdater();
   m_Root = &GetRootTransform();

   SetDirty();
}

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

// will need to implment a world position stays version too.
void Transform::SetParent( Transform& parent )
{
   m_Parent->UnRegisterChild( m_ToThisInParentsChildList );
   m_Parent = &parent;
   m_ToThisInParentsChildList = m_Parent->RegisterChild( *this );

   SetDirty();
}

void Transform::ClearParent()
{
   SetParent( *m_Root );
}

void Transform::InitParent( Transform& parent )
{
   // this is called before awake
   m_Parent = &parent;
   m_ToThisInParentsChildList = m_Parent->RegisterChild( *this );
}

mat4 Transform::GetTransformMatrix()
{
   if (m_Dirty)
   {
      Debug::Instance().LogError( "Attempt to get global matrix when dirty" );
   }

   return m_TransformMatrix;
}

void Transform::SetDirty()
{
   if (!m_Dirty)
   {
      m_Dirty = true;
      m_TransformUpdater->AddDirtyTransform( *this );
   }
}

void Transform::Clean()
{
   if (m_Parent->IsDirty())
      Debug::Instance().LogError( "Cleaning when parent is dirty" );

   m_Position = m_Parent->GetPositionUnconditional() + m_LocalPosition.Value();
   m_Rotation = m_LocalRotation.Value() * m_Parent->GetRotationUnconditional();

   // TODO - construct the matrix
  // m_TransformMatrix = ConstructMatrix( m_Position, m_Rotation );

   m_Dirty = false;

   for (std::list<Transform*>::iterator it = m_Childern.begin; it != m_Childern.end; it++)
   {
      (*it)->Clean();
   }
}

Transform& Transform::FindCleanHierarchy()
{
   Transform* clean = this;
   Transform* current = this;

   do
   {
      if (clean)
      {
         if (current->IsDirty())
         {
            clean = nullptr;
         }
      }
      else
      {
         if (!current->IsDirty())
         {
            clean = current;
         }
      }

      current = &(current->GetParent());

   } while (current != nullptr);

   if (!clean)
   {
      Debug::Instance().LogError( "Unable to find clean hierarchy" );
   }

   return *clean;
}

vec3 Transform::GetLocalPosition()
{
   return m_LocalPosition.Value();
}

vec3 Transform::GetPosition()
{
   Transform *clean = &FindCleanHierarchy();
   Transform *current = this;
   vec3 pos = vec3( 0.0f, 0.0f, 0.0f );

   while (current != clean)
   {
      pos += current->GetLocalPosition();
      current = &(current->GetParent());
   }

   pos += clean->GetPositionUnconditional();

   return pos;
}

vec3 Transform::GetPositionUnconditional()
{
   return m_Position;
}

Quaternion Transform::GetRotation()
{
   Transform *clean = &FindCleanHierarchy();
   Transform *current = this;
   Quaternion rot = QuaternionIdentity();

   while (current != clean)
   {
      rot = rot * current->GetLocalRotation();
      current = &(current->GetParent());
   }

   rot = rot * clean->GetPositionUnconditional();

   return rot;
}

Quaternion Transform::GetLocalRotation()
{
   return m_LocalRotation.Value();
}

Quaternion Transform::GetRotationUnconditional()
{
   return m_Rotation;
}


vec3 Transform::SetLocalPosition( vec3 pos )
{
   m_LocalPosition.SetValue( pos );
   SetDirty();
}

vec3 Transform::SetPosition( vec3 pos )
{
   m_LocalPosition.SetValue( pos - m_Parent->GetPosition() );
   SetDirty();
}

Quaternion Transform::SetRotation( Quaternion rot )
{
   m_LocalRotation.SetValue( rot * QuaternionConj( m_Parent->GetRotation() ) );
   SetDirty();
}

Quaternion Transform::SetLocalRotation( Quaternion rot )
{
   m_LocalRotation.SetValue( rot );
   SetDirty();
}

Transform& Transform::GetParent()
{
   return *m_Parent;
}

bool Transform::IsDirty()
{
   return m_Dirty;
}
