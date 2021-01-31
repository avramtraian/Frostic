#pragma once

#include "Frostic/Core/Layer.h"

#include "Frostic/Events/ApplicationEvent.h"
#include "Frostic/Events/MouseEvent.h"
#include "Frostic/Events/KeyEvent.h"

namespace Frostic {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

	};

}