#include <vector>

#include "stdafx.h"
#include "MathLib.h"
#include "glm.hpp"
#include <iostream>
#include "Utils.h"
#include <sstream>
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

	// Test OK
	int * graham_scan(float * points, int nb_point, int* out_size)
	{
		if (nb_point < 3) return nullptr;

		std::vector<glm::vec2> v_points;
		for (int i = 0; i < nb_point; i++) {
			v_points.push_back(glm::vec2(points[i * 2], points[i * 2+ 1]));
		}

		glm::vec2 barycenter = Utils::get_barycenter(v_points);

		std::list<int> sorted_points = Utils::graham_sort(barycenter, v_points);
		std::list<int>::iterator it = sorted_points.begin();

		int start = *it;
		int pivot = start;
		bool forward = false;

		do {
			glm::vec2 prev, next;
			if (it == sorted_points.begin()) {
				prev = Utils::get_vector_from_points(v_points[pivot], v_points[*std::prev(sorted_points.end())]);
				next = Utils::get_vector_from_points(v_points[pivot], v_points[*std::next(it)]);
			}
			else if (it == std::prev(sorted_points.end())) {
				prev = Utils::get_vector_from_points(v_points[pivot], v_points[*std::prev(it)]);
				next = Utils::get_vector_from_points(v_points[pivot], v_points[*sorted_points.begin()]);
			}
			else {
				prev = Utils::get_vector_from_points(v_points[pivot], v_points[*std::prev(it)]);
				next = Utils::get_vector_from_points(v_points[pivot], v_points[*std::next(it)]);
			}

			float angle = Utils::oriented_angle_2PI(prev, next);
			if (angle > PI) {
				if (it == std::prev(sorted_points.end()))
					it = sorted_points.begin();
				else
					++it;

				forward = true;
			}
			else {
				if (it == sorted_points.begin()) {
					it = std::prev(sorted_points.end());
					sorted_points.erase(sorted_points.begin());
					start = *it;
				}
				else {
					--it;
					sorted_points.erase(std::next(it));
				}

				forward = false;
			}

			if (it != sorted_points.end())
				pivot = *it;

		} while (pivot != start || forward == false);

		*out_size = sorted_points.size();

		int * result_in_int = new int[sorted_points.size()]();
		int i = 0;
		for (std::list<int>::iterator f_it = sorted_points.begin(); f_it != sorted_points.end(); ++f_it) {
			result_in_int[i] = *f_it;
			i++;
		}
		return result_in_int;
	}

	float* incremental_triangulation(std::vector<glm::vec2> points) {
		//poly page 26
		std::vector<glm::vec2*> triangulation;
		if (points.size() <= 2) {
			glm::vec2* triangle = new glm::vec2[3]();
			triangle[0] = points.front();
			triangle[1] = points.front();
			if(points.size() == 2) triangle[2] = points.back();
			else triangle[2] = points.front();
			triangulation.push_back(triangle);
			return Utils::convert_from_triangulation(triangulation);
		}
		//1)
		std::list<glm::vec2> sorted_points = Utils::triangulate_sort(points);
		//2)a)
		std::list<glm::vec2> aligned_points;
		aligned_points.push_back(*sorted_points.begin());
		aligned_points.push_back(*std::next(sorted_points.begin()));
		std::list<glm::vec2>::iterator next_point;
		for (std::list<glm::vec2>::iterator f_it = sorted_points.begin(); f_it != std::prev(sorted_points.end(), 2); ++f_it) {
			if (Utils::is_colinear(Utils::get_vector_from_points(*f_it, *std::next(f_it)), Utils::get_vector_from_points(*std::next(f_it), *std::next(f_it, 2)))) {
				aligned_points.push_back(*std::next(f_it, 2));
				//si tous les points sont alignés
				if (f_it == std::prev(sorted_points.end(), 2)) {
					glm::vec2* triangle = new glm::vec2[3]();
					triangle[0] = points.front();
					triangle[1] = points.front();
					triangle[2] = points.back();
					triangulation.push_back(triangle);
					return Utils::convert_from_triangulation(triangulation);
				}
			}
			else {
				//Pk+1
				next_point = std::next(f_it, 2);
				break;
			}
		}
		//2)b)
		for (std::list<glm::vec2>::iterator f_it = aligned_points.begin(); f_it != std::prev(aligned_points.end()); ++f_it) {
			glm::vec2* triangle = new glm::vec2[3]();
			triangle[0] = *f_it;
			triangle[1] = *std::next(f_it);
			triangle[2] = *next_point;
			triangulation.push_back(triangle);
		}
		next_point = std::next(next_point);
		//3)
		for (std::list<glm::vec2>::iterator f_it = next_point; f_it != std::prev(sorted_points.end()); ++f_it) {
			//3)a)
			std::list<Utils::edge*> convex_envelope = Utils::get_convex_envelope(Utils::convert_from_triangulation(triangulation), triangulation.size()*3);
			std::vector<Utils::edge*> visible_edges = Utils::get_visible_edges(*next_point, convex_envelope, triangulation);
			//3)b)
			for (auto edge : visible_edges) {
				glm::vec2* triangle = new glm::vec2[3]();
				triangle[0] = edge->s1;
				triangle[1] = edge->s2;
				triangle[2] = *next_point;
				triangulation.push_back(triangle);
			}
		}
		return Utils::convert_from_triangulation(triangulation);
		//(où les 6 premiers floats sont p1x, p1y, p2x, p2y, p3x, p3y) avec p1p2p3 le premier triangle 
	}

#pragma region tests

	std::string debug_vec2(glm::vec2 pt) {
		std::ostringstream os;
		os << "x : " << pt.x << " | y: " << pt.y << " ";
		return os.str();
	}

	/*bool test_incremental_triangulation_size2() {
		glm::vec2 p1(1, 1);
		glm::vec2 p2(-1, -1);
		std::vector<glm::vec2> tab;
		tab.push_back(p1);
		tab.push_back(p2);
		std::vector<glm::vec2*> result = incremental_triangulation(tab);
		std::vector<glm::vec2*> triangulation;
		glm::vec2 triangle[3];
		triangle[0] = p1;
		triangle[1] = p2;
		triangle[2] = p2;
		triangulation.push_back(triangle);
		//std::cout << "result : " << debug_vec2(result[0][0]) << debug_vec2(result[0][1]) << debug_vec2(result[0][2]) << std::endl;
		//std::cout << "triangle : " << debug_vec2(triangulation[0][0]) << debug_vec2(triangulation[0][1]) << debug_vec2(triangulation[0][2]) << std::endl;
		return (result[0][0] == triangulation[0][0] && result[0][1] == triangulation[0][1] && result[0][2] == triangulation[0][2]);
	}*/

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

	/*bool test_graham_sort() {
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
				std::cout << "x : " << debug_results.front() << " | y: " << debug_results.front() << std::endl;
				debug_results.pop_front();
			}
			return false;
		}
	}*/

	MATHLIB_API void test()	{
		/*std::cout << "test du barycentre : " << std::boolalpha << test_barycenter() << std::endl;
		std::cout << "test du produit scalaire : " << std::boolalpha << test_scalar_product() << std::endl;
		std::cout << "test de la norme : " << std::boolalpha << test_norm() << std::endl;
		std::cout << "test du get vector from points : " << std::boolalpha << test_vector_from_points() << std::endl;
		std::cout << "test du oriented angle : " << std::boolalpha << test_oriented_angle() << std::endl;
		std::cout << "test du oriented angle 2PI: " << std::boolalpha << test_oriented_angle_2PI() << std::endl;
		std::cout << "test du graham sort : " << std::boolalpha << test_graham_sort() << std::endl;
		std::cout << "test de l'incrementation triangulaire pour 2 points : " << std::boolalpha << test_incremental_triangulation_size2() << std::endl;*/
	}
#pragma endregion