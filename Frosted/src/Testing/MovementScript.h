#pragma once

#include <Frostic.h>

#include "ScriptedEntity.h"

namespace Frostic {

	class MovementScript : public ScriptableEntity
	{
	public:
		MovementScript() { FE_SCRIPT_CLASS(758943); PushProperties(); }
		void PushProperties();

		virtual void Begin() override;
		virtual void Tick(Timestep ts) override;
	private:
		void Move(Timestep ts);
	public:
		float m_XVelocity = 1.5f;
		float m_YVelocity = 1.0f;

		ScriptedEntity* se = nullptr;
	};

}