#pragma once

#include <Frostic.h>

namespace Frostic {

	class EnemyScript : public ScriptableEntity
	{
	public:
		EnemyScript();
	private:
		virtual void PushProperties() override;
	};

}