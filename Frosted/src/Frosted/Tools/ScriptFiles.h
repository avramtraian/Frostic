#pragma once

#include "Frostic/Core/Core.h"

#include <string>

/////////////////////////////////////////////////////////////////////
/////////////***  THIS CODE IS EXTREMLY SENSITIVE  ***///////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//////***  MODIFY THIS ONLY IF YOU KNOW WHAT ARE YOU DOING  ***//////
/////////////////////////////////////////////////////////////////////

namespace Frostic {

	class ScriptFiles
	{
	public:
		ScriptFiles() = default;

		void BeginGenerate();
		void BeginDelete();
		void End();

		void OnImGuiRender();

		void GenerateFiles();
		void DeleteFiles();
		void GenerateScriptManagerFile(uint64_t id);
		void DeleteSciptManagerFile(uint64_t id);

		bool IsActive() const { return m_Active; }
		bool IsGenerating() const { return m_GenerateFiles; }
		bool IsDeleting() const { return m_DeleteFiles; }
	private:
		bool m_Active = false;
		bool m_GenerateFiles = false;
		bool m_DeleteFiles = false;
		uint64_t m_ScriptID = 0;
		std::string m_ScriptName = std::string();
		std::string m_HFilepath = std::string();
		std::string m_CPPFilepath = std::string();
	};

}