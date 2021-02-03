#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Core/KeyCodes.h"
#include "Frostic/Core/MouseButtonCodes.h"

namespace Frostic {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);

		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

		static void SetHiddenCursor(bool lock);
	};

}