#include <vector>

#include "stdafx.h"
#include "MathLib.h"
#include "glm.hpp"

namespace Mathlib
{
	int classic_pow(int a, int p) 
	{
		for (int i = 1; i < p; i++, a *= a);
		return a;
	}

	float * jarvis_walk(float * points, int nb_point) 
	{
		if (nb_point < 3) return nullptr;

		std::vector<glm::vec2> result;

		float xmin = points[0]; float ymin = points[1]; glm::vec2 start = glm::vec2(xmin, ymin);
		for (int i = 1; i < nb_point; i++) {
			if (points[i * 2] < xmin || (points[i * 2] == xmin && points[i * 2 + 1] < ymin)) {
				xmin = points[i * 2]; ymin = points[i * 2 + 1];
				start = glm::vec2(xmin, ymin);
			}
		}

		glm::vec2 right_support = glm::vec2(0.0f, -1.0f);
		result.push_back(start);
	}
}