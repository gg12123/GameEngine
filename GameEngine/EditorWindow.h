#pragma once

class Editor;

class EditorWindow
{
public:
   virtual void Awake( Editor& editor ) = 0;
   virtual void Update() = 0;
};
