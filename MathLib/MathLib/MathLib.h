#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

#include "glm.hpp"

namespace Mathlib {

	extern "C" 
	{
		MATHLIB_API int classic_pow(int a, int p);

		MATHLIB_API int * jarvis_walk(float * points, int nb_point, int* out_size);
	}

}