#include "MainToolWindow.h"
#include "ImGUI/imgui.h"

MainToolWindow::MainToolWindow()
{
   m_Editor = nullptr;
}

void MainToolWindow::Awake( Editor& editor )
{
   m_Editor = &editor;
}

void MainToolWindow::Update()
{
   ImGui::Begin( "Editor", 0, ImGuiWindowFlags_AlwaysAutoResize );

   m_SceneManagement.OnGUI( *m_Editor );

   ImGui::End();
}