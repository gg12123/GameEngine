#pragma once
#include "EditorWindow.h"
#include "EditorUtils.h"
#include "ParentSetter.h"

class GameObject;

class HierarchyWindow : public EditorWindow
{
public:
   HierarchyWindow();

   void Awake( Editor& editor ) override;
   void Update() override;

private:
   void DrawTreeView( GameObject& obj, int id );
   void ContextMenu();

   Editor* m_Editor;
   ParentSetter m_ParentSetter;
   GameObjectCreationContext m_GOCreation;
   PrefabSpawnSaveContext m_PrefabSpawn;
   GameObjectRenamer m_GORenamer;
};
