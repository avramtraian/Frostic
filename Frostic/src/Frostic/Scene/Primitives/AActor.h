#pragma once

#include "FObject.h"

namespace Frostic {

	class AActor : public FObject
	{
	public:
		bool m_Active = true;
	};

}