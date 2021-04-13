#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Scene/Scene.h"
#include "Frostic/Renderer/Texture.h"

namespace Frostic {

	class AddScriptComponent
	{
	public:
		AddScriptComponent() = default;

		void Begin();
		void End();

		void OnImGuiRender();

		void GenerateFiles();
		void GenerateScriptManagerFile(uint64_t id);
	private:
		bool m_Active = false;
		std::string m_ScriptName = std::string("NewNativeScript");
		std::string m_HFilepath = std::string("C:/dev/Frostic-dev/Frosted/src/AddScriptTest/" + m_ScriptName + ".h");
		std::string m_CPPFilepath = std::string("C:/dev/Frostic-dev/Frosted/src/AddScriptTest/" + m_ScriptName + ".cpp");
	};

	class ToolsPanel
	{
	public:
		ToolsPanel();

		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();
	private:
		Ref<Scene> m_Context;

		// 0 - play button
		// 1 - stop button
		std::array<Ref<Texture2D>, 2> m_Icons;

		AddScriptComponent m_AddScriptComponent;
	};

}