#pragma once

#include <Frostic.h>

namespace Frostic {

	class PlayerScript : public ScriptableEntity
	{
	public:
		PlayerScript();

		void Jump();
	private:
		virtual void PushProperties() override;

		virtual void Begin() override;
		virtual void Tick(Timestep ts) override;
	private:
		float m_JumpForce = 0.5f;
	};

}