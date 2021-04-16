#include "PlayerScript.h"

namespace Frostic {

	PlayerScript::PlayerScript()
	{
		FE_SCRIPT_CLASS(9252044);
		PushProperties();
	}

	void PlayerScript::Jump()
	{
		PhysicsComponent2D* physics = GetComponentPointer<PhysicsComponent2D>();
		if(physics != nullptr)
			physics->AddForce(m_JumpForce, { 0.0f, 1.0f });
	}

	void PlayerScript::PushProperties()
	{
		FE_DATA_PROPERTY(m_JumpForce, "Jump Force", DataType::FLOAT);
	}

	void PlayerScript::Begin()
	{
		Entity entity = GetScene()->CreateEntity("mmmm noice yessss");
		entity.AddComponent<SpriteRendererComponent>();
		entity.GetComponent<TransformComponent>().Translation.x = 1.5f;
	}

	void PlayerScript::Tick(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::Space))
			Jump();
	}

}