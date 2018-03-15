#pragma once
#include <stack>
#include <string>
#include <vector>
#include "MyVmath.h"

class GameObject;
class Transform;

class EnumerableHierarchy
{
public:
   EnumerableHierarchy( GameObject& root );

   GameObject* Next();

private:
   std::stack<GameObject*> m_Stack;
};

class TwoDimentionalSpace
{
public:
   TwoDimentionalSpace( const float halfWidth, const float halfHeight, const vmath::vec2 centre );

public:
   float HalfWidth() const;
   float HalthHeight() const;
   vmath::vec2 Centre() const;

private:
   float m_HalfWidth;
   float m_HalfHeight;
   vmath::vec2 m_Centre;
};

GameObject& DeSerializeHierarchy( std::ifstream& stream, std::vector<GameObject*>& gameObjects );

void SerializeHierarchy( GameObject& root, std::ofstream& stream );

GameObject& DuplicateHierarchy( GameObject& root, std::vector<GameObject*>& duplicatedObjs );

GameObject& DuplicateHierarchy( GameObject& root, const std::vector<GameObject*>& gameObjects, std::vector<GameObject*>& duplicatedObjs );

GameObject& HierarchyForNewProject( std::vector<GameObject*>& gameObjects );

GameObject& CreateLightGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent );

GameObject& CreateCameraGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent );

GameObject& CreateCubeGameObject( vmath::vec3 pos, vmath::mat4 rot, Transform& parent );

void CopyStringToBuffer( char *buffer, const std::string str );

void GetFileNamesInDirectory( const std::string dir, std::vector<std::string>& names );

vmath::vec2 Transform2DDirection( const TwoDimentionalSpace& sourceSpace,
                                  const TwoDimentionalSpace& destSpace,
                                  const vmath::vec2& sourceDir );

vmath::vec2 Transform2DPoint( const TwoDimentionalSpace& sourceSpace,
                              const TwoDimentionalSpace& destSpace,
                              const vmath::vec2& sourcePoint );