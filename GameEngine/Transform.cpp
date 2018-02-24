#include "Transform.h"
#include "Debug.h"
#include "TransformUpdater.h"
#include "ComponentIDs.h"

Transform::Transform()
{
   m_Parent = nullptr;
}

void Transform::Awake()
{
   m_TransformUpdater = &GetTransformUpdater();
   m_Root = &GetRootTransform();

   ConstructLocalTransformMatrix();
   SetDirty();
}

std::list<Transform*>::iterator Transform::ChildrenBegin()
{
   return m_Childern.begin();
}

std::list<Transform*>::iterator Transform::ChildrenEnd()
{
   return m_Childern.end();
}

std::list<Transform*>::iterator Transform::RegisterChild( Transform& child )
{
   m_Childern.push_front(&child);
   return m_Childern.begin();
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

mat4 Transform::GetTransformMatrixAssumingClean()
{
   if (m_Dirty)
   {
      // note: not checking parents because it is too inefficient.
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

   m_TransformMatrix = m_Parent->GetTransformMatrixAssumingClean() * m_LocalTransformMatrix;

   m_Dirty = false;

   for (std::list<Transform*>::iterator it = m_Childern.begin(); it != m_Childern.end(); it++)
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

void Transform::ConstructTransformMatrix()
{
   Transform *clean = &FindCleanHierarchy();
   Transform *current = this;

   // dont modify the member transform matrix until the end because
   // its original value maybe required in the final equation.
   mat4 transformMatrix = mat4().identity();

   while (current != clean)
   {
      transformMatrix = current->GetLocalTransformMatrix() * transformMatrix;
      current = &(current->GetParent());
   }

   m_TransformMatrix = current->GetTransformMatrixAssumingClean() * transformMatrix;
}

void Transform::ConstructLocalTransformMatrix()
{
   m_LocalTransformMatrix = m_LocalRotation.Value();

   m_LocalTransformMatrix[ 3 ] = vec4( m_LocalPosition.Value()[ 0 ],
                                       m_LocalPosition.Value()[ 1 ],
                                       m_LocalPosition.Value()[ 2 ],
                                       1.0f );
}

vec3 Transform::GetPosition()
{
   ConstructTransformMatrix();
   
   return vec3( m_TransformMatrix[ 3 ][ 0 ], m_TransformMatrix[ 3 ][ 1 ], m_TransformMatrix[ 3 ][ 2 ] );
}

mat4 Transform::GetRotation()
{
   ConstructTransformMatrix();
   
   mat4 rot = m_TransformMatrix;

   rot[ 3 ][ 0 ] = 0.0f;
   rot[ 3 ][ 1 ] = 0.0f;
   rot[ 3 ][ 2 ] = 0.0f;

   return rot;
}

mat4 Transform::GetLocalTransformMatrix()
{
   return m_LocalTransformMatrix;
}

mat4 Transform::GetLocalRotation()
{
   return m_LocalRotation.Value();
}

void Transform::SetLocalPosition( vec3 pos )
{
   m_LocalPosition.SetValue( pos );
   ConstructLocalTransformMatrix();
   SetDirty();
}

void Transform::SetLocalRotation( mat4 rot )
{
   m_LocalRotation.SetValue( rot );
   ConstructLocalTransformMatrix();
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

int32_t Transform::GetType()
{
   return COMPONENT_ID_TRANSFORM;
}

std::string Transform::GetName()
{
   return "Transform";
}

void Transform::GetSerializedFields( std::unordered_map<std::string, SerializedField*>& fields )
{
   fields[ "position" ] = &m_LocalPosition;
   fields[ "rotation" ] = &m_LocalRotation;
}