#pragma once
#include <list>
#include "Transform.h"

class TransformUpdater
{
public:

   void AddDirtyTransform( Transform& dirtyTransform );

   // called in update, just before rendering
   void UpdateTransforms();

private:

   Transform& FindRootDirtyParent(Transform& child);

   std::list<Transform*> m_DirtyTransforms;
};
