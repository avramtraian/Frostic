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
		if (physics != nullptr)
			physics->AddForce(m_JumpForce, { 0.0f, 1.0f });
	}

	void PlayerScript::MoveSideways(int8_t direction)
	{
		PhysicsComponent2D* physics = GetComponentPointer<PhysicsComponent2D>();
		if (physics != nullptr)
			physics->AddForce(m_SidewaysForce, { direction, 0 });
	}

	void PlayerScript::PushProperties()
	{
		FE_DATA_PROPERTY(m_JumpForce, "Jump Force", DataType::FLOAT, AccesModifiers::ReadAndWrite);
		FE_DATA_PROPERTY(m_SidewaysForce, "Sideways Force", DataType::FLOAT, AccesModifiers::ReadAndWrite);
		FE_DATA_STRUCTURE_PROPERTY(name, "Name", DataStructureType::FEStringType, DataType::None, AccesModifiers::ReadAndWrite);
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
		if (Input::IsKeyPressed(Key::D))
			MoveSideways(1);
		else if (Input::IsKeyPressed(Key::A))
			MoveSideways(-1);
	}

}