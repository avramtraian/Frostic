#pragma once

#include <Frostic.h>

namespace Frostic {

	class PlayerScript : public ScriptableEntity
	{
	public:
		PlayerScript();

		void Jump();
		void MoveSideways(int8_t direction);
	private:
		virtual void PushProperties() override;

		virtual void Begin() override;
		virtual void Tick(Timestep ts) override;
	private:
		float m_JumpForce = 0.5f;
		float m_SidewaysForce = 0.5f;
		FEString name = "mmmm yessss";
	};

}