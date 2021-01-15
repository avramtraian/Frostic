#pragma once

#include "Frostic/Layer.h"

#include "Frostic/Events/ApplicationEvent.h"
#include "Frostic/Events/MouseEvent.h"
#include "Frostic/Events/KeyEvent.h"

namespace Frostic {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;

	};

}