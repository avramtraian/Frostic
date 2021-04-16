#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Frostic/Core/Log.h"

#include "Frostic/Debug/Instrumentor.h"

#ifdef FE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif