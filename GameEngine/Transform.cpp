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

   if (m_Parent != nullptr)
   {
      // If not the root, set dirty so that global transform will be updated.
      SetDirty();
   }
   else
   {
      // The roots global transform will equal its local and it will never change
      m_TransformMatrix = m_LocalTransformMatrix;
   }

   if (m_Parent == nullptr && (m_Root != this))
   {
      throw std::exception( "Parent is null" );
   }
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

void Transform::SetParent( Transform& parent )
{
   ConstructTransformMatrix();
   mat4 original = m_TransformMatrix; 

   m_Parent->UnRegisterChild( m_ToThisInParentsChildList );
   m_Parent = &parent;
   m_ToThisInParentsChildList = m_Parent->RegisterChild( *this );

   SetPosition( extractPositionOnly( original ) );
   SetRotation( toMat4( extractRotationOnly( original ) ) );
   SetScale( extractScaleOnly( original ) );
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

vec3 Transform::GetLocalScale()
{
   return m_LocalScale.Value();
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

mat4 Transform::GetTransformMatrix()
{
   ConstructTransformMatrix();
   return m_TransformMatrix;
}

void Transform::ConstructLocalTransformMatrix()
{
   m_LocalTransformMatrix = m_LocalRotation.Value() * scale( m_LocalScale.Value() );

   m_LocalTransformMatrix[ 3 ] = vec4( m_LocalPosition.Value()[ 0 ],
                                       m_LocalPosition.Value()[ 1 ],
                                       m_LocalPosition.Value()[ 2 ],
                                       1.0f );
}

void Transform::SetPosition( vec3 pos )
{
   mat4 parentsTransform = m_Parent->GetTransformMatrix();

   vec3 parPos = extractPositionOnly( parentsTransform );
   vec3 parRightScaled = extractRightOnly( parentsTransform );
   vec3 parUpScaled = extractUpOnly( parentsTransform );
   vec3 parForwardScaled = extractForwardOnly( parentsTransform );

   vec3 parToMe = pos - parPos;

   SetLocalPosition( vec3( dot( parToMe, normalize( parRightScaled ) ) / length( parRightScaled ),
                           dot( parToMe, normalize( parUpScaled ) ) / length( parUpScaled ),
                           dot( parToMe, normalize( parForwardScaled ) ) / length( parForwardScaled ) ) );
}

void Transform::SetRotation( mat4 rot )
{
   mat3 parentsRot = extractRotationOnly( m_Parent->GetTransformMatrix() );
   mat3 rotOnly = extractRotationOnly( rot );

   mat3 requiredLocalRot = parentsRot.transpose() * rotOnly;

   SetLocalRotation( toMat4( requiredLocalRot ) );
}

void Transform::SetScale( vec3 s )
{
   mat3 parRotScaled = extractRotationScaled( m_Parent->GetTransformMatrix() );
   mat3 unscaledTransform = parRotScaled * extractRotationOnly( m_LocalTransformMatrix );

   SetLocalScale( vec3( s[ 0 ] / length( unscaledTransform[ 0 ] ),
                        s[ 1 ] / length( unscaledTransform[ 1 ] ),
                        s[ 2 ] / length( unscaledTransform[ 2 ] ) ) );
}

vec3 Transform::GetPosition()
{
   ConstructTransformMatrix();
   return extractPositionOnly( m_TransformMatrix );
}

mat4 Transform::GetRotation()
{
   ConstructTransformMatrix();
   return toMat4( extractRotationOnly( m_TransformMatrix ) );
}

vec3 Transform::GetScale()
{
   ConstructTransformMatrix();
   return extractScaleOnly( m_TransformMatrix );
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

void Transform::SetLocalScale( vec3 s )
{
   m_LocalScale.SetValue( s );
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
   fields[ "scale" ] = &m_LocalScale;
}