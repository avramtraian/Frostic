#include "ToolsPanel.h"

#include "Frostic/Scene/SceneManager.h"

#include "Frosted/Tools/BuildWindows.h"

#include <imgui/imgui.h>

namespace Frostic {

	ToolsPanel::ToolsPanel()
	{
		m_Icons[0] = Texture2D::Create(FILEPATH("Frosted/internal/toolsIcons/Start.png"));
		m_Icons[1] = Texture2D::Create(FILEPATH("Frosted/internal/toolsIcons/Stop.png"));
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
			BuildWindows::Get().CopyFiles("C:/dev/Frostic-gen");
		}

		ImGui::SameLine();
		if (ImGui::Button("Add NativeScript", ImVec2{ 50, 50 }))
		{
			if (m_ScriptFiles.IsDeleting())
			{
				m_ScriptFiles.End();
				m_ScriptFiles.BeginGenerate();
			}
			else if (m_ScriptFiles.IsGenerating())
			{
				m_ScriptFiles.End();
			}
			else
			{
				m_ScriptFiles.BeginGenerate();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete NativeScript", ImVec2{ 50, 50 }))
		{
			if (m_ScriptFiles.IsGenerating())
			{
				m_ScriptFiles.End();
				m_ScriptFiles.BeginDelete();
			}
			else if (m_ScriptFiles.IsDeleting())
			{
				m_ScriptFiles.End();
			}
			else
			{
				m_ScriptFiles.BeginDelete();
			}
		}

		ImGui::End();

		m_ScriptFiles.OnImGuiRender();
	}

}