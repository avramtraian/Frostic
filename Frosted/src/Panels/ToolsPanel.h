#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Core/Log.h"
#include "Frostic/Scene/Scene.h"

namespace Frostic {
	class ToolsPanel
	{
	public:
		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();
	private:
		Ref<Scene> m_Context;

		bool m_Runtime = false;
	};

}