#pragma once

#ifdef MATHLIB_EXPORTS
#define MATHLIB_API __declspec(dllexport)
#else
#define MATHLIB_API __declspec(dllimport)
#endif

namespace Mathlib {

	extern "C" 
	{
		MATHLIB_API int classic_pow(int a, int p);

		MATHLIB_API float * jarvis_walk(float * points, int nb_point);

		MATHLIB_API void test();
	}

}