#include "ScriptedEntity.h"

namespace Frostic {

	ScriptedEntity::ScriptedEntity()
	{
		PushProperties();
	}

	void ScriptedEntity::PushProperties()
	{
		FE_DATA_PROPERTY(m_XVelocity, "X Velocity", DataType::FLOAT);
		FE_DATA_PROPERTY(m_YVelocity, "Y Velocity", DataType::FLOAT);
	}

	void ScriptedEntity::Begin()
	{

	}

	void ScriptedEntity::Tick(Timestep ts)
	{
		Move(ts);
	}

	void ScriptedEntity::Move(Timestep ts)
	{
		TransformComponent& tc = GetComponent<TransformComponent>();
		if (Input::IsKeyPressed(Key::A))
			tc.Translation.x -= m_XVelocity * ts;
		else if (Input::IsKeyPressed(Key::D))
			tc.Translation.x += m_XVelocity * ts;
		if (Input::IsKeyPressed(Key::S))
			tc.Translation.y -= m_YVelocity * ts;
		else if (Input::IsKeyPressed(Key::W))
			tc.Translation.y += m_YVelocity * ts;
	}

}