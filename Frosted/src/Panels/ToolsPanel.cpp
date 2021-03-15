#include "ToolsPanel.h"

#include "Frostic/Scene/SceneManager.h"

#include <imgui/imgui.h>

namespace Frostic {

	ToolsPanel::ToolsPanel()
	{
		m_Icons[0] = Texture2D::Create(FILEPATH("internal/toolsIcons/Start.png"));
		m_Icons[1] = Texture2D::Create(FILEPATH("internal/toolsIcons/Stop.png"));
	}

	void ToolsPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void ToolsPanel::OnImGuiRender()
	{
		ImGui::Begin("Tools");
		
		if (!SceneManagerEditor::IsRuntime())
		{
			if (ImGui::ImageButton((void*)m_Icons[0]->GetRendererID(), ImVec2{ 44, 44 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				SceneManagerEditor::StartRuntimeScene(m_Context);
			}
		}
		else
		{
			if (ImGui::ImageButton((void*)m_Icons[1]->GetRendererID(), ImVec2{ 44, 44 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				SceneManagerEditor::StopRuntimeScene();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Build", ImVec2{ 50, 50 }))
		{

		}

		ImGui::End();
	}

}