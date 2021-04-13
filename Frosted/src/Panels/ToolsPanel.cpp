#include "ToolsPanel.h"

#include "Frostic/Scene/SceneManager.h"
#include "Frostic/Script/ScriptManager.h"
#include "Frostic/Script/ScriptSerializer.h"
#include "Frostic/Math/Random.h"

#include <string>
#include <fstream>

#include <cstdlib>

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

		}

		ImGui::SameLine();
		if (ImGui::Button("Add NativeScript", ImVec2{ 50, 50 }))
		{
			m_AddScriptComponent.Begin();
		}

		ImGui::End();

		m_AddScriptComponent.OnImGuiRender();
	}


	void AddScriptComponent::Begin()
	{
		m_Active = true;
	}

	void AddScriptComponent::End()
	{
		m_Active = false;
	}

	void AddScriptComponent::OnImGuiRender()
	{
		if (m_Active)
		{
			ImGui::Begin("Add Script");

			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 120.0f);

			ImGui::Text("Class name: ");
			ImGui::NextColumn();

			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), m_ScriptName.c_str());
				if (ImGui::InputText("##classname", buffer, sizeof(buffer)))
				{
					if (!m_HFilepath.empty() && m_HFilepath.find(m_ScriptName + ".h") != std::string::npos)
					{
						for (size_t i = 0; i < m_ScriptName.size() + 2; i++)
							m_HFilepath.pop_back();
						m_HFilepath.append(std::string(buffer) + ".h");
					}
					if (!m_CPPFilepath.empty() && m_CPPFilepath.find(m_ScriptName + ".cpp") != std::string::npos)
					{
						for (size_t i = 0; i < m_ScriptName.size() + 4; i++)
							m_CPPFilepath.pop_back();
						m_CPPFilepath.append(std::string(buffer) + ".cpp");
					}
					m_ScriptName = buffer;
				}
			}
			ImGui::Columns(1);
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 120.0f);

			ImGui::Text("Header Filepath: ");
			ImGui::NextColumn();
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), m_HFilepath.c_str());
				if (ImGui::InputText("##hfilepath", buffer, sizeof(buffer)))
					m_HFilepath = buffer;
			}
			ImGui::Columns(1);
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, 120.0f);

			ImGui::Text("CPP Filepath: ");
			ImGui::NextColumn();
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), m_CPPFilepath.c_str());
				if (ImGui::InputText("##cppfilepath", buffer, sizeof(buffer)))
					m_CPPFilepath = buffer;
			}

			ImGui::Columns(1);

			ImGui::SameLine(ImGui::GetWindowSize().x / 2 - 75.0f);
			if (ImGui::Button("Generate class files...", ImVec2{ 150.0f, 50.0f }))
			{
				GenerateFiles();
			}

			ImGui::End();
		}
	}

	void AddScriptComponent::GenerateFiles()
	{
		std::ofstream hfile(m_HFilepath);
		std::ofstream cppfile(m_CPPFilepath);

		char quotationMark = 34;

		hfile << "#pragma once" << std::endl << std::endl;
		hfile << "#include <Frostic.h>" << std::endl << std::endl;
		hfile << "namespace Frostic {" << std::endl << std::endl;
		hfile << "	class " << m_ScriptName << " : public ScriptableEntity" << std::endl;
		hfile << "	{" << std::endl;
		hfile << "	public:" << std::endl;
		hfile << "		" << m_ScriptName << "();" << std::endl;
		hfile << "	private:" << std::endl;
		hfile << "		" << "virtual void PushProperties() override;" << std::endl;
		hfile << "	" << "};" << std::endl << std::endl;
		hfile << "}";

		Math::Random::Init();
		uint64_t scriptID = Math::Random::Range<uint64_t>(1, 100000000000000);

		cppfile << "#include " << quotationMark << m_ScriptName << ".h" << quotationMark << std::endl << std::endl;
		cppfile << "namespace Frostic {" << std::endl << std::endl;
		cppfile << "	" << m_ScriptName << "::" << m_ScriptName << "()" << std::endl;
		cppfile << "	" << "{" << std::endl;
		cppfile << "		" << "FE_SCRIPT_CLASS(" << std::to_string(scriptID) << ");" << std::endl;
		cppfile << "		" << "PushProperties();" << std::endl;
		cppfile << "	" << "}" << std::endl << std::endl;
		cppfile << "	" <<"void " << m_ScriptName << "::PushProperties()" << std::endl;
		cppfile << "	" << "{" << std::endl;
		cppfile << "		" << std::endl;
		cppfile << "	" << "}" << std::endl << std::endl;
		cppfile << "}";

		hfile.close();
		cppfile.close();

		GenerateScriptManagerFile(scriptID);

		ScriptManager::AddScript(scriptID, m_ScriptName);
		ScriptSerializer serializer;
		serializer.Serialize(FILEPATH("Frosted/assets/scripts/Scripts.fescript"));

#ifdef FE_PLATFORM_WINDOWS
		std::system("C:/dev/Frostic-dev/scripts/Win-GenProjects.bat");
#endif
	}

	void AddScriptComponent::GenerateScriptManagerFile(uint64_t id)
	{
		std::ifstream infilestream(FILEPATH("Frostic/src/Frostic/Script/ScriptManager.cpp"));
		std::stringstream ss;
		ss << infilestream.rdbuf();
		std::string file = ss.str();

		char quotationMark = 34;

		size_t pos = file.find(std::string("default"));
		std::stringstream switchss;
		switchss << "" << "case " << std::to_string(id) << ":" << std::endl;
		switchss << "				" << "function = []() { return static_cast<ScriptableEntity*>(new " << m_ScriptName << "()); };" << std::endl;
		switchss << "				" << "break;" << std::endl << "			";
		file.insert(pos, switchss.str());

		std::ofstream outfilestream(FILEPATH("Frostic/src/Frostic/Script/ScriptManager.cpp"));
		outfilestream << file;

		std::ifstream ifheaders(FILEPATH("Frostic/src/Frostic/Script/ScriptsHeaders.h"));
		std::stringstream headers;
		headers << ifheaders.rdbuf();
		headers << "#include " << quotationMark << m_HFilepath << quotationMark << std::endl;
		std::ofstream outheaders(FILEPATH("Frostic/src/Frostic/Script/ScriptsHeaders.h"));
		outheaders << headers.str();
	}

}