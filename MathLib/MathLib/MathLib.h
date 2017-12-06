#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

#include "glm.hpp"

namespace Mathlib {

	float jarvis_angle(glm::vec2, glm::vec2);

	float norme(glm::vec2);

	extern "C" 
	{
		MATHLIB_API int classic_pow(int a, int p);

		MATHLIB_API float * jarvis_walk(float * points, int nb_point);
	}

}