#pragma once

#include <Frostic.h>

namespace Frostic {

	class ScriptedEntity : public ScriptableEntity
	{
	public:
		ScriptedEntity();
		virtual void PushProperties() override;

		virtual void Begin() override;
		virtual void Tick(Timestep ts) override;
	private:
		void Move(Timestep ts);
	public:
		float m_XVelocity = 1.5f;
		float m_YVelocity = 1.0f;
	};

}