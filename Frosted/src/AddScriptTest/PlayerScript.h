#pragma once

#include <Frostic.h>

namespace Frostic {

	class PlayerScript : public ScriptableEntity
	{
	public:
		PlayerScript();

		void Jump(Timestep ts);
		void AddForce(float force);
	private:
		virtual void PushProperties() override;

		virtual void Begin() override;
		virtual void Tick(Timestep ts) override;
	private:
		float m_YVelocity = 0.0f;
		float m_YAcceleration = 0.0f;
		float m_Force = 0.0f;
		float m_Gravity = 10.0f;
	};

}