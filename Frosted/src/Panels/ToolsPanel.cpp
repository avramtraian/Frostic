#include "ToolsPanel.h"

#include "Frostic/Scene/SceneManager.h"

#include <imgui/imgui.h>

namespace Frostic {

	void ToolsPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void ToolsPanel::OnImGuiRender()
	{
		ImGui::Begin("Tools");
		
		if (!m_Runtime)
		{
			if (ImGui::Button("Play", ImVec2{ 50, 50 }))
			{
				m_Runtime = true;
				SceneManagerEditor::StartRuntimeScene(m_Context);
			}
		}
		else
		{
			if (ImGui::Button("Stop", ImVec2{ 50, 50 }))
			{
				m_Runtime = false;
				SceneManagerEditor::StopRuntimeScene();
			}
		}

		ImGui::End();
	}

}