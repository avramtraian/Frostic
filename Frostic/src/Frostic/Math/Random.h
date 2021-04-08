#pragma once

#include <random>

namespace Frostic::Math
{

	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		template<typename TypeGen>
		static TypeGen Range(TypeGen min, TypeGen max)
		{
			return (TypeGen)(Float() * max + min);
		}

	private:
		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}