#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Scene/Scene.h"
#include "Frostic/Renderer/Texture.h"

namespace Frostic {
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
	};

}