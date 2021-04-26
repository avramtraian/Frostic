#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Core/Log.h"
#include "Frostic/Scene/ScriptableEntity.h"

#include "Frostic/Core/FEArray.h"
#include "Frostic/Core/FEString.h"
#include "Frostic/Core/FEVector.h"

namespace Frostic::GUI {

	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

	bool DrawFloatControl(const std::string& label, float* value, float columnWidth = 100.0f);
	bool DrawFloatRangeControl(const std::string& label, float* value, float min, float max, float columnWidth = 100.0f);

	// TODO: Integrate proper casting for these (from unsigned -> int)
	bool DrawUInt8_tControl(const std::string& label, uint8_t* value, float columnWidth = 100.0f);
	bool DrawUInt16_tControl(const std::string& label, uint16_t* value, float columnWidth = 100.0f);
	bool DrawUInt32_tControl(const std::string& label, uint32_t* value, float columnWidth = 100.0f);
	bool DrawUInt64_tControl(const std::string& label, uint64_t* value, float columnWidth = 100.0f);
	// TODO: Integrate proper casting for these (from unsigned -> int)
	bool DrawInt8_tControl(const std::string& label, int8_t* value, float columnWidth = 100.0f);
	bool DrawInt16_tControl(const std::string& label, int16_t* value, float columnWidth = 100.0f);
	bool DrawInt32_tControl(const std::string& label, int32_t* value, float columnWidth = 100.0f);
	bool DrawInt64_tControl(const std::string& label, int64_t* value, float columnWidth = 100.0f);
	bool DrawIntControl(const std::string& label, int* value, float columnWidth = 100.0f);

	void DrawEntityReferenceField(const std::string& label, Entity& entRef, const Ref<Scene>& context, float columnWidth = 100.0f);
	ScriptableEntity* DrawScriptPointerField(const std::string& label, ScriptableEntity* se, uint64_t scriptID, const Ref<Scene>& context, float columnWidth = 100.0f);

	bool DrawCheckbox(const std::string& label, bool* value, float columnWidth = 100.0f);

	void DrawFEArray(const std::string& label, void* data, DataType dataType, float columnWidth = 100.0f);
	void DrawFEString(const std::string& label, FEString& string, float columnWidth = 100.0f);
	
}