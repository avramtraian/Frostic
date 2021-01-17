#pragma once

#include "Frostic/Core.h"
#include "Frostic/Core/Timestep.h"
#include "Frostic/Events/Event.h"

namespace Frostic {

	class FROSTIC_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline std::string GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}