#pragma once

#include <memory>

#ifdef FE_PLATFORM_WINDOWS
	#ifdef FE_DYNAMIC_LINK
		#ifdef FE_BUILD_DLL
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

#ifdef FE_ENABLE_ASSERTS
	#define FE_CORE_ASSERT(x, ...) { if(!(x)) { FE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define FE_ASSERT(x, ...) { if(!(x)) { FE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else 
	#define FE_CORE_ASSERT(x, ...)
	#define FE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define FILEPATH(fp) std::string("C:/dev/Frostic-dev/Frosted/").append(fp)

namespace Frostic {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	constexpr T* Cast(Args&& ... args)
	{
		return dynamic_cast<T*>(std::forward<Args>(args)...);
	}
	
}