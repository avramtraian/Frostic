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
	public:
		void Print();
	public:
		int ValueToPrint = 0;
		Entity entity;
	};

}