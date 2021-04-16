#include "ScriptFiles.h"

#include "Frostic/Core/Log.h"

#include "Frostic/Script/ScriptManager.h"
#include "Frostic/Script/ScriptSerializer.h"

#include "Frostic/Math/Random.h"

#include <string>
#include <fstream>

#include <cstdlib>
#include <stdio.h>
#include <filesystem>

#include <imgui/imgui.h>

/////////////////////////////////////////////////////////////////////
/////////////***  THIS CODE IS EXTREMLY SENSITIVE  ***///////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//////***  MODIFY THIS ONLY IF YOU KNOW WHAT ARE YOU DOING  ***//////
/////////////////////////////////////////////////////////////////////

namespace Frostic {

	void ScriptFiles::BeginGenerate()
	{
		m_Active = true;
		m_GenerateFiles = true;
		m_DeleteFiles = false;
		m_ScriptName = std::string("NewNativeScript");
		m_HFilepath = std::string("C:/dev/Frostic-dev/Sandbox/src/AddScriptTest/" + m_ScriptName + ".h");
		m_CPPFilepath = std::string("C:/dev/Frostic-dev/Sandbox/src/AddScriptTest/" + m_ScriptName + ".cpp");
	}

	void ScriptFiles::BeginDelete()
	{
		m_Active = true;
		m_DeleteFiles = true;
		m_GenerateFiles = false;
	}

	void ScriptFiles::End()
	{
		m_Active = false;
		m_GenerateFiles = false;
		m_DeleteFiles = false;

		m_ScriptID = 0;

		m_ScriptName = std::string();
		m_HFilepath = std::string();
		m_CPPFilepath = std::string();
	}

	void ScriptFiles::OnImGuiRender()
	{
		if (m_Active)
		{
			if (m_GenerateFiles)
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

				if (m_ScriptName.size() != 0 && m_HFilepath.size() != 0 && m_CPPFilepath.size() != 0)
				{
					ImGui::SameLine(ImGui::GetWindowSize().x / 2 - 75.0f);
					if (ImGui::Button("Generate class files...", ImVec2{ 150.0f, 50.0f }))
					{
						if (!ScriptManager::Exists(m_ScriptName))
							GenerateFiles();
					}
				}

				ImGui::End();
			}
			else if (m_DeleteFiles)
			{
				ImGui::Begin("Delete Script Files");

				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, 120.0f);
				ImGui::Text("Script to delete: ");
				ImGui::NextColumn();
				if (ImGui::Button(m_ScriptName == "" ? "None" : m_ScriptName.c_str()))
				{
					ImGui::OpenPopup("deletescript");
				}
				if (ImGui::BeginPopup("deletescript"))
				{
					ScriptManager::Each([&](uint64_t id, ScriptManager::Data& data)
						{
							if (ImGui::MenuItem(data.Name.c_str()))
							{
								m_ScriptName = data.Name;
								m_HFilepath = data.HFilepath;
								m_CPPFilepath = data.CPPFilepath;
								m_ScriptID = id;
							}
						});

					if (ImGui::MenuItem("None"))
					{
						m_ScriptName = "";
						m_HFilepath = "";
						m_CPPFilepath = "";
					}

					ImGui::EndPopup();
				}

				ImGui::Columns(1);

				if (m_ScriptName.size() != 0 && m_HFilepath.size() != 0 && m_CPPFilepath.size() != 0 && m_ScriptID != 0)
				{
					ImGui::SameLine(ImGui::GetWindowSize().x / 2 - 75.0f);
					if (ImGui::Button("Delete class files...", ImVec2{ 150.0f, 50.0f }))
					{
						DeleteFiles();
					}
				}

				ImGui::End();
			}
		}
	}

	void ScriptFiles::GenerateFiles()
	{
		if (!std::filesystem::exists(m_HFilepath) && !std::filesystem::exists(m_CPPFilepath))
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
			m_ScriptID = Math::Random::Range<uint64_t>(1, 100000000000000);

			cppfile << "#include " << quotationMark << m_ScriptName << ".h" << quotationMark << std::endl << std::endl;
			cppfile << "namespace Frostic {" << std::endl << std::endl;
			cppfile << "	" << m_ScriptName << "::" << m_ScriptName << "()" << std::endl;
			cppfile << "	" << "{" << std::endl;
			cppfile << "		" << "FE_SCRIPT_CLASS(" << std::to_string(m_ScriptID) << ");" << std::endl;
			cppfile << "		" << "PushProperties();" << std::endl;
			cppfile << "	" << "}" << std::endl << std::endl;
			cppfile << "	" << "void " << m_ScriptName << "::PushProperties()" << std::endl;
			cppfile << "	" << "{" << std::endl;
			cppfile << "		" << std::endl;
			cppfile << "	" << "}" << std::endl << std::endl;
			cppfile << "}";

			hfile.close();
			cppfile.close();

			FE_CORE_INFO("Generated .h & .cpp files...");

			GenerateScriptManagerFile(m_ScriptID);

			ScriptManager::AddScript(m_ScriptID, m_ScriptName, m_HFilepath, m_CPPFilepath);
			ScriptSerializer serializer;
			serializer.Serialize(FILEPATH("Frosted/assets/scripts/Scripts.fescript"));

#ifdef FE_PLATFORM_WINDOWS
			std::system("C:/dev/Frostic-dev/scripts/Win-GenProjects.bat");
#endif

			FE_CORE_INFO("Script '{0}'(ID: {1}) created succesfully!", m_ScriptName, m_ScriptID);
		}
		else
		{
			FE_CORE_ERROR("Path already exists!");
		}
		End();
	}

	void ScriptFiles::GenerateScriptManagerFile(uint64_t id)
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

		FE_CORE_INFO("Updated ScriptManger.cpp file...");

		std::ifstream ifheaders(FILEPATH("Frostic/src/Frostic/Script/ScriptsHeaders.h"));
		std::stringstream headers;
		headers << ifheaders.rdbuf();
		headers << "#include " << quotationMark << m_HFilepath << quotationMark << std::endl;
		std::ofstream outheaders(FILEPATH("Frostic/src/Frostic/Script/ScriptsHeaders.h"));
		outheaders << headers.str();

		FE_CORE_INFO("Updated ScriptHeaders.h file...");
	}

	void ScriptFiles::DeleteFiles()
	{
		uint8_t hstatus, cppstatus;
		hstatus = std::remove(m_HFilepath.c_str());
		cppstatus = std::remove(m_CPPFilepath.c_str());
		if (hstatus != 0)
			FE_CORE_ERROR("Unable to delete file: {0}", m_HFilepath);
		if (cppstatus != 0)
			FE_CORE_ERROR("Unable to delete file: {0}", m_CPPFilepath);

		FE_CORE_INFO("Deleted .h & .cpp files...");

		ScriptManager::RemoveScript(m_ScriptID);
		ScriptSerializer serializer;
		serializer.Serialize(FILEPATH("Frosted/assets/scripts/Scripts.fescript"));

		DeleteSciptManagerFile(m_ScriptID);

#ifdef FE_PLATFORM_WINDOWS
		std::system("C:/dev/Frostic-dev/scripts/Win-GenProjects.bat");
#endif

		FE_CORE_INFO("Script '{0}'(ID: {1}) was deleted succesfully!", m_ScriptName, m_ScriptID);
		End();
	}

	void ScriptFiles::DeleteSciptManagerFile(uint64_t id)
	{
		std::ifstream infilestream;
		std::ofstream outfilestream;

		char quotationMark = 34;

		{
			infilestream.open(FILEPATH("Frostic/src/Frostic/Script/ScriptsHeaders.h"), std::ifstream::in);
			std::stringstream ss;
			ss << infilestream.rdbuf();
			std::string str = ss.str();
			std::stringstream toDelete;
			toDelete << "#include " << quotationMark << m_HFilepath << quotationMark << std::endl;
			size_t pos = str.find(toDelete.str());
			if (pos != std::string::npos)
			{
				str.erase(pos, toDelete.str().size());
				outfilestream.open(FILEPATH("Frostic/src/Frostic/Script/ScriptsHeaders.h"), std::ofstream::out);
				outfilestream << str;
			}
			infilestream.close();
			outfilestream.close();

			FE_CORE_INFO("Updated ScriptHeaders.h file...");
		}
		{
			infilestream.open(FILEPATH("Frostic/src/Frostic/Script/ScriptManager.cpp"));
			std::stringstream ss;
			ss << infilestream.rdbuf();
			std::string str = ss.str();
			std::stringstream toDel;
			toDel << std::endl << "			case " << id << ":" << std::endl << "				function = []() { return static_cast<ScriptableEntity*>(new " << m_ScriptName << "()); };" << std::endl << "				break;";
			size_t pos = str.find(toDel.str());
			if (pos != std::string::npos)
			{
				str.erase(pos, toDel.str().size());
				outfilestream.open(FILEPATH("Frostic/src/Frostic/Script/ScriptManager.cpp"), std::ofstream::out);
				outfilestream << str;
			}
			infilestream.close();
			outfilestream.close();

			FE_CORE_INFO("Updated ScriptManage.cpp file...");
		}
	}

}