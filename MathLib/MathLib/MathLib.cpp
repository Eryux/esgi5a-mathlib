#include <vector>

#include "stdafx.h"
#include "MathLib.h"
#include "Utils.h"

#include <iostream>

namespace Mathlib
{
	int classic_pow(int a, int p) 
	{
		for (int i = 1; i < p; i++, a *= a);
		return a;
	}

	void test(int& a) {
		a = 5;
	}

	// Test OK
	int * jarvis_walk(float * points, int nb_point, int * out_size) 
	{
		if (nb_point < 3) return nullptr;

		std::vector<int> result;

		float xmin = points[0]; float ymin = points[1]; int start = 0;
		for (int i = 1; i < nb_point; i++) {
			if (points[i * 2] < xmin || (points[i * 2] == xmin && points[i * 2 + 1] < ymin)) {
				xmin = points[i * 2]; ymin = points[i * 2 + 1];
				start = i;
			}
		}

		glm::vec2 right_support = glm::vec2(0.0f, -1.0f);

		int i = start; 
		int j = 0;

		do {
			result.push_back(i * 2);
			result.push_back(i * 2 + 1);

			j = (i == 0) ? 1 : 0;

			glm::vec2 vec = Utils::get_vector_from_points(glm::vec2(points[i * 2], points[i * 2 + 1]), glm::vec2(points[j * 2], points[j * 2 + 1]));
			float angle_min = Utils::jarvis_angle(right_support, vec);
			float lmax = Utils::norm(vec);
			int inew = j;

			for (j = inew + 1; j < nb_point; j++) 
			{
				if (j != i) 
				{
					glm::vec2 vec_2 = Utils::get_vector_from_points(glm::vec2(points[i * 2], points[i * 2 + 1]), glm::vec2(points[j * 2], points[j * 2 + 1]));
					float angle = Utils::jarvis_angle(right_support, vec_2);
					float l = Utils::norm(vec_2);
					if (angle_min > angle || (angle_min == angle && lmax < l))
					{
						angle_min = angle;
						lmax = l;
						inew = j;
					}
				}
			}

			right_support = glm::vec2(points[inew * 2] - points[i * 2], points[inew * 2 + 1] - points[i * 2 + 1]);
			i = inew;
		} while (i != start);

		*out_size = result.size();

		int * result_in_int = new int[*out_size]();
		for (int i = 0; i < *out_size; result_in_int[i] = result[i], i++);
		return (result_in_int);
	}
}