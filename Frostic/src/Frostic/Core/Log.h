#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Frostic {

	class FROSTIC_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define FE_CORE_TRACE(...)     ::Frostic::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FE_CORE_INFO(...)      ::Frostic::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FE_CORE_WARN(...)      ::Frostic::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FE_CORE_ERROR(...)     ::Frostic::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FE_CORE_FATAL(...)     ::Frostic::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define FE_TRACE(...)   ::Frostic::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FE_INFO(...)    ::Frostic::Log::GetClientLogger()->info(__VA_ARGS__)
#define FE_WARN(...)    ::Frostic::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FE_ERROR(...)   ::Frostic::Log::GetClientLogger()->error(__VA_ARGS__)
#define FE_FATAL(...)   ::Frostic::Log::GetClientLogger()->fatal(__VA_ARGS__)