#include "TransformUpdater.h"

void TransformUpdater::AddDirtyTransform( Transform& dirtyTransform )
{
   m_DirtyTransforms.push_back( &dirtyTransform );
}

Transform& TransformUpdater::FindRootDirtyParent( Transform& child )
{
   Transform* dirty = &child; // assume the input is dirty
   Transform* current = dirty;

   while (current != nullptr)
   {
      if (current->IsDirty())
      {
         dirty = current;
      }

      current = &(current->GetParent());
   }

   return *dirty;
}

void TransformUpdater::UpdateTransforms()
{
   for (std::vector<Transform*>::iterator it = m_DirtyTransforms.begin; it != m_DirtyTransforms.end; it++)
   {
      Transform *t = (*it);

      if (t->IsDirty())
      {
         Transform& d = FindRootDirtyParent( *t );
         d.Clean();
      }
   }

   m_DirtyTransforms.clear();
}