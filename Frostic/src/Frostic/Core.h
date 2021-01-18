#pragma once

#include <memory>

#ifdef FR_PLATFORM_WINDOWS
	#ifdef FR_DYNAMIC_LINK
		#ifdef FR_BUILD_DLL
			#define FROSTIC_API  __declspec(dllexport)
		#else
			#define FROSTIC_API  __declspec(dllimport)
		#endif
	#else 
		#define FROSTIC_API  
	#endif
#else
	#error Frostic only supports Windows!
#endif

#ifdef FR_ENABLE_ASSERTS
	#define FR_CORE_ASSERT(x, ...) { if(!(x)) { FR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define FR_ASSERT(x, ...) { if(!(x)) { FR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else 
	#define FR_CORE_ASSERT(x, ...)
	#define FR_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Frostic {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}