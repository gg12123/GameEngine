#pragma once
#include <vector>

class Transform;

class TransformUpdater
{
public:

   void AddDirtyTransform( Transform& dirtyTransform );

   // called in update, just before rendering
   void UpdateTransforms();

private:

   Transform& FindRootDirtyParent(Transform& child);

   std::vector<Transform*> m_DirtyTransforms;
};
