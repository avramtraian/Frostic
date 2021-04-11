#include "MovementScript.h"

namespace Frostic {
	
	void MovementScript::PushProperties()
	{
		FE_DATA_PROPERTY(m_XVelocity, "X Velocity", DataType::FLOAT);
		FE_DATA_PROPERTY(m_YVelocity, "Y Velocity", DataType::FLOAT);

		FE_SCRIPT_PROPERTY(se, "Scripted Entity", ScriptedEntity);
	}

	void MovementScript::Begin()
	{

	}

	void MovementScript::Tick(Timestep ts)
	{
		Move(ts);

		if (se != nullptr)
			se->Print();
	}

	void MovementScript::Move(Timestep ts)
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