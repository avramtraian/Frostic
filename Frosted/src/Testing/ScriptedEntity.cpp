#include "ScriptedEntity.h"

#include "MovementScript.h"

namespace Frostic {

	ScriptedEntity::ScriptedEntity()
	{
		FE_SCRIPT_CLASS(759834);
		PushProperties();
	}

	void ScriptedEntity::PushProperties()
	{
		FE_DATA_PROPERTY(ValueToPrint, "Value To Print", DataType::INT);
		FE_ENTITY_REFERENCE_PROPERTY(entity, "Entity To Print");
	}

	void ScriptedEntity::Begin()
	{

	}

	void ScriptedEntity::Tick(Timestep ts)
	{
		
	}

	void ScriptedEntity::Print()
	{
		if (entity != NULL_ENTITY)
		{
			MovementScript* ms = entity.GetNativeComponent<MovementScript>();
			if (ms != nullptr)
				FE_WARN("Velocity is x: {0}, y: {1}", ms->m_XVelocity, ms->m_YVelocity);
		}
		FE_WARN("Ce fac cu viata mea?");
	}

}