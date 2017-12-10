#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

#include "glm.hpp"

#include "Utils.h"

namespace Mathlib {

	Utils::triangulation* incremental_triangulation(std::vector<glm::vec2> points);

	extern "C" 
	{
		MATHLIB_API int classic_pow(int a, int p);

		MATHLIB_API void test();

		MATHLIB_API int * jarvis_walk(float * points, int nb_point, int* out_size);

		MATHLIB_API int * graham_scan(float * points, int nb_point, int* out_size);

		MATHLIB_API float * triangulate(float * points, int nb_point, int * out_size);
	}

}