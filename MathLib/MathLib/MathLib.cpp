#include <vector>

#include "stdafx.h"
#include "MathLib.h"
#include "glm.hpp"
#include <iostream>
#include "Utils.h"
#define PI 3.14159265359


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

	bool test_barycenter() {
		glm::vec2 p1(1, 1);
		glm::vec2 p2(-1, -1);
		std::vector<glm::vec2> tab;
		tab.push_back(p1);
		tab.push_back(p2);
		glm::vec2 result(0, 0);
		return result == Utils::get_barycenter(tab);
	}

	bool test_scalar_product() {
		glm::vec2 p1(2, 3);
		glm::vec2 p2(-4, 5);
		float result = 7;
		return result == Utils::scalar_product(p1, p2);
	}

	bool test_norm() {
		glm::vec2 p1(6, 8);
		float result = 10;
		return result == Utils::norm(p1);
	}

	bool test_vector_from_points() {
		glm::vec2 p1(2, 3);
		glm::vec2 p2(-4, 5);
		glm::vec2 result(-6, 2);
		return result == Utils::get_vector_from_points(p1, p2);
	}

	bool test_oriented_angle() {
		glm::vec2 p1(2, 3);
		glm::vec2 p2(-4, 5);
		if (Utils::oriented_angle(p1, p2) != Utils::jarvis_angle(p1, p2)) {
			std::cout << "probleme oriented angle : != jarvis_angle" << std::endl;
			return false;
		}
		return Utils::oriented_angle(p1, p2) == -Utils::oriented_angle(p2, p1);
	}

	bool test_oriented_angle_2PI() {
		glm::vec2 p1(2, 3);
		glm::vec2 p2(-4, 5);
		if (Utils::oriented_angle_2PI(p1, p2) != Utils::oriented_angle(p1, p2)) {
			std::cout << "probleme oriented angle 2PI : angles positifs" << std::endl;
			return false;
		}
		return ((2 * PI - Utils::oriented_angle(p1, p2)) - Utils::oriented_angle_2PI(p2, p1)) <0.0001f;
	}

	bool test_graham_sort() {
		std::vector<glm::vec2> tab;
		glm::vec2 p3(1, 1);	tab.push_back(p3);
		glm::vec2 p1(3, 3);	tab.push_back(p1);
		glm::vec2 p4(3, 1);	tab.push_back(p4);
		glm::vec2 p2(1, 3);	tab.push_back(p2);

		glm::vec2 bary(2, 2);
		if (Utils::get_barycenter(tab) != bary) {
			std::cout << "probleme graham sort : barycentre" << std::endl;
			return false;
		}
		std::list<glm::vec2> result; 
		result.push_back(p1);
		result.push_back(p2);
		result.push_back(p3);
		result.push_back(p4);
		std::list<glm::vec2> calculated_result = Utils::graham_sort(bary, tab);
		std::list<glm::vec2> debug_results = calculated_result;
		int count = debug_results.size();
		if (result != calculated_result) {
			for (int i = 0; i < count; i++) {
				std::cout << "x : " << debug_results.front().x << " | y: " << debug_results.front().y << std::endl;
				debug_results.pop_front();
			}
			return false;
		}
	}

	MATHLIB_API void test()
	{
		std::cout << "test du barycentre : " << std::boolalpha << test_barycenter() << std::endl;
		std::cout << "test du produit scalaire : " << std::boolalpha << test_scalar_product() << std::endl;
		std::cout << "test de la norme : " << std::boolalpha << test_norm() << std::endl;
		std::cout << "test du get vector from points : " << std::boolalpha << test_vector_from_points() << std::endl;
		std::cout << "test du oriented angle : " << std::boolalpha << test_oriented_angle() << std::endl;
		std::cout << "test du oriented angle 2PI: " << std::boolalpha << test_oriented_angle_2PI() << std::endl;
		std::cout << "test du graham sort : " << std::boolalpha << test_graham_sort() << std::endl;
	}
}

