#include "PlayerScript.h"

namespace Frostic {

	PlayerScript::PlayerScript()
	{
		FE_SCRIPT_CLASS(9252044);
		PushProperties();
	}

	void PlayerScript::Jump(Timestep ts)
	{
		AddForce(50.0f);
	}

	void PlayerScript::AddForce(float force)
	{
		if (force != 0)
		{
			m_Force += force;
		}
	}

	void PlayerScript::PushProperties()
	{
		FE_DATA_PROPERTY(m_YVelocity, "Y Velocity", DataType::FLOAT);
		FE_DATA_PROPERTY(m_YAcceleration, "Y Acceleration", DataType::FLOAT);
		FE_DATA_PROPERTY(m_Gravity, "Gravity", DataType::FLOAT);
	}

	void PlayerScript::Begin()
	{

	}

	void PlayerScript::Tick(Timestep ts)
	{
		TransformComponent& tc = GetComponent<TransformComponent>();

		if (Input::IsKeyPressed(Key::Space))
			Jump(ts);

		AddForce(-m_Gravity * 1.0f);

		m_YAcceleration = m_Force / 1.0f;
		m_YVelocity += m_YAcceleration * ts;
		tc.Translation.y += m_YVelocity * ts;

		m_Force = 0;
	}

}