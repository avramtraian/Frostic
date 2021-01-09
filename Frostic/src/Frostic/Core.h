#pragma once

#ifdef FR_PLATFORM_WINDOWS
	#ifdef FROSTIC_BUILD_DLL
		#define FROSTIC_API __declspec(dllexport)
	#else
		#define FROSTIC_API __declspec(dllimport)
	#endif
#else
	#error Frostic only supports Windows!
#endif