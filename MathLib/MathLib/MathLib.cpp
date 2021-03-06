#include <vector>
#include <algorithm>
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

	// ------------------------------------------

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

	// ------------------------------------------

	// Test OK
	int * graham_scan(float * points, int nb_point, int* out_size)
	{
		if (nb_point < 3) return nullptr;

		std::vector<glm::vec2> v_points;
		for (int i = 0; i < nb_point; i++) {
			v_points.push_back(glm::vec2(points[i * 2], points[i * 2 + 1]));
		}

		glm::vec2 barycenter = Utils::get_barycenter(v_points);

		std::list<int> sorted_points = Utils::graham_sort(barycenter, v_points);
		std::list<int>::iterator it = sorted_points.begin();

		int start = *it;
		int pivot = start;
		bool forward = false;
		int d = 0;

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
				if (*it == start) {
					it = std::next(sorted_points.begin());
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

			d++;
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

	// ------------------------------------------

	Utils::triangulation* incremental_triangulation(std::vector<glm::vec2> points) {
		//poly page 26
		if (points.size() <= 2) {
			std::cerr << "pas assez de points dans la triangulation" << std::endl;
			return nullptr;
		}
		//1)
		std::list<glm::vec2> sorted_points = Utils::triangulate_sort(points);
		Utils::triangulation* triangulation = new Utils::triangulation();
		//2)a)
		std::list<Utils::edge*> aligned_edges;
		Utils::edge* first_edge = new Utils::edge(*sorted_points.begin(), *std::next(sorted_points.begin()));
		aligned_edges.push_back(first_edge);
		std::list<glm::vec2>::iterator next_point;
		for (std::list<glm::vec2>::iterator f_it = sorted_points.begin(); f_it != std::prev(sorted_points.end(), 2); ++f_it) {
			if (Utils::is_colinear(*first_edge, Utils::edge(*std::next(f_it), *std::next(f_it, 2)))) {
				aligned_edges.push_back(new Utils::edge(*std::next(f_it), *std::next(f_it, 2)));
				//si tous les points sont align�s
				if (f_it == std::prev(sorted_points.end(), 3)) {
					Utils::triangle* triangle = new Utils::triangle(new Utils::edge(sorted_points.front(), sorted_points.back()), new Utils::edge(sorted_points.front(), sorted_points.back()), new Utils::edge(sorted_points.front(), sorted_points.back()));
					std::cout << "tous les points sont align�s" << std::endl;
					std::vector<Utils::edge*> edge_list(aligned_edges.begin(), aligned_edges.end());
					triangulation->edge_list = edge_list;
					std::vector<Utils::triangle*> triangle_list;
					triangle_list.push_back(triangle);
					triangulation->triangle_list = triangle_list;
					return triangulation;
				}
			}
			else {
				//Pk+1
				next_point = std::next(f_it, 2);
				break;
			}
		}
		//2)b)
		std::vector<Utils::edge*> edge_list;
		std::vector<Utils::triangle*> triangle_list;
		//initialisation du premier edge form� avec le nouveau point
		Utils::edge* my_edge = new Utils::edge(*sorted_points.begin(), *next_point);
		for (std::list<glm::vec2>::iterator f_it = sorted_points.begin(); f_it != std::prev(next_point); ++f_it) {
			//chaque point cr�� les edges dont il a besoin pour faire les triangles qu'il touche
			//il n'ajoute que le triangle qu'il genere avec le point suivant 
			//et les deux ar�tes de ce triangle qui partent de lui (sauf le dernier)
			Utils::edge* next_point_edge = new Utils::edge(*f_it, *std::next(f_it));
			Utils::edge* next_new_point_edge = new Utils::edge(*std::next(f_it), *next_point);
			Utils::triangle* triangle = new Utils::triangle(my_edge, next_point_edge, next_new_point_edge);
			if (my_edge->t1 == nullptr) my_edge->t1 = triangle; //normalement ce cas n'est que pour le premier point
			else my_edge->t2 = triangle; //cas g�n�ral
			next_point_edge->t1 = triangle;
			next_new_point_edge->t1 = triangle;
			edge_list.push_back(my_edge);
			edge_list.push_back(next_point_edge);
			triangle_list.push_back(triangle);
			my_edge = next_new_point_edge;
		}
		//dernier edge form� avec le nouveau point
		edge_list.push_back(my_edge);


		next_point = std::next(next_point);
		//3)
		for (std::list<glm::vec2>::iterator f_it = next_point; f_it != sorted_points.end(); ++f_it) {
			//3)a)
			std::list<Utils::edge*> convex_envelope = Utils::get_convex_envelope(edge_list);
			std::vector<Utils::edge*> visible_edges = Utils::get_visible_edges(*f_it, convex_envelope, edge_list);
			//3)b)
			for (Utils::edge* edge : visible_edges) {
				Utils::edge* e1_new = new Utils::edge(edge->s1, *f_it);
				Utils::edge* e2_new = new Utils::edge(edge->s2, *f_it);
				Utils::triangle* triangle = new Utils::triangle(edge, e1_new, e2_new);
				//est ce que ma edge est deja dans la liste? -> je r�cup�re sa position dans index
				int index = Utils::find(edge_list, e1_new);
				if (index != -1) {
					// oui : on lui file le triangle en t2 (elle a forc�ment deja un t1)
					edge_list[index]->t2 = triangle;
				}
				else {
					//non : on lui met le triangle en t1 et on la met dedans
					e1_new->t1 = triangle;
					edge_list.push_back(e1_new);
				}
				//idem pour la deuxieme edge
				index = Utils::find(edge_list, e2_new);
				if (index != -1) {
					edge_list[index]->t2 = triangle;
				}
				else {
					e2_new->t1 = triangle;
					edge_list.push_back(e2_new);
				}
				edge->t2 = triangle;
				triangle_list.push_back(triangle);
			}
			/*Utils::edge* s1_new = nullptr;
			Utils::edge* s2_new = nullptr;
			for (Utils::edge* edge : visible_edges) {
				if(s2_new == nullptr) s1_new = new Utils::edge(edge->s1, *f_it);
				else s1_new = s2_new;
				if(edge->s1 == s1_new->s1) s2_new = new Utils::edge(edge->s2, *f_it);
				else s2_new = new Utils::edge(edge->s1, *f_it);
				Utils::triangle* triangle = new Utils::triangle(edge, s1_new, s2_new);
				if(s1_new->t1 != nullptr) s1_new->t2 = triangle;
				else s1_new->t1 = triangle;
				s2_new->t1 = triangle;
				edge->t2 = triangle;
				if(edge_list.back() != s1_new) edge_list.push_back(s1_new);
				edge_list.push_back(s2_new);
				triangle_list.push_back(triangle);
			}*/
		}
		triangulation->edge_list = edge_list;
		triangulation->triangle_list = triangle_list;
		return triangulation;
		//(o� les 6 premiers floats sont p1x, p1y, p2x, p2y, p3x, p3y) avec p1p2p3 le premier triangle 
	}

	// ------------------------------------------

	float * triangulate(float * points, int nb_point, int * out_size) 
	{
		std::vector<glm::vec2> vec_points;
		for (int i = 0; i < nb_point; i++) { vec_points.push_back(glm::vec2(points[i * 2], points[i * 2 + 1])); }

		Utils::triangulation * t = incremental_triangulation(vec_points);

		std::vector<float> result;
		for (int i = 0; i < t->triangle_list.size(); i++) {
			result.push_back(t->triangle_list[i]->a1->s1.x);
			result.push_back(t->triangle_list[i]->a1->s1.y);
			result.push_back(t->triangle_list[i]->a1->s2.x);
			result.push_back(t->triangle_list[i]->a1->s2.y);
			glm::vec2 third_vertex = Utils::get_third_vertex(t->triangle_list[i], t->triangle_list[i]->a1);
			result.push_back(third_vertex.x);
			result.push_back(third_vertex.y);
		}

		*out_size = result.size();

		float * result_in_float = new float[*out_size]();
		for (int i = 0; i < *out_size; i++) { result_in_float[i] = result[i]; }
		delete t;

		return result_in_float;
	}

	// ------------------------------------------

	float * triangulate_delaunay(float * points, int nb_point, int * out_size)
	{
		std::vector<glm::vec2> vec_points;
		for (int i = 0; i < nb_point; i++) { vec_points.push_back(glm::vec2(points[i * 2], points[i * 2 + 1])); }

		Utils::triangulation * t = incremental_triangulation(vec_points);
		Utils::edge_flipping(t->edge_list);

		std::vector<float> result;
		for (int i = 0; i < t->triangle_list.size(); i++) {
			result.push_back(t->triangle_list[i]->a1->s1.x);
			result.push_back(t->triangle_list[i]->a1->s1.y);
			result.push_back(t->triangle_list[i]->a1->s2.x);
			result.push_back(t->triangle_list[i]->a1->s2.y);
			result.push_back(Utils::get_third_vertex(t->triangle_list[i], t->triangle_list[i]->a1).x);
			result.push_back(Utils::get_third_vertex(t->triangle_list[i], t->triangle_list[i]->a1).y);
		}

		*out_size = result.size();

		float * result_in_float = new float[*out_size]();
		for (int i = 0; i < *out_size; i++) { result_in_float[i] = result[i]; }
		delete t;

		return result_in_float;
	}

	// ------------------------------------------

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

	void test_triangulation() {
		glm::vec2 p1(0, 0);
		glm::vec2 p2(0, 2);
		glm::vec2 p3(0, 3);
		glm::vec2 p4(1, 3);
		glm::vec2 p5(2, 2.8);
		glm::vec2 p6(1.3, 1);
		glm::vec2 p7(3, 2);
		glm::vec2 p8(3.2, 1);
		glm::vec2 p9(3, -1);
		glm::vec2 p10(5, 2.5);
		std::vector<glm::vec2> tab;
		tab.push_back(p1);
		tab.push_back(p2);
		tab.push_back(p3);
		tab.push_back(p4);
		tab.push_back(p5);
		tab.push_back(p6);
		tab.push_back(p7);
		tab.push_back(p8);
		tab.push_back(p9);
		tab.push_back(p10);
		incremental_triangulation(tab);
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
		return ((2 * PI - Utils::oriented_angle(p1, p2)) - Utils::oriented_angle_2PI(p2, p1)) < 0.0001f;
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

	void test_flipping() 
	{
		glm::vec2 p1(0, 1);
		glm::vec2 p2(0, -1);
		glm::vec2 p3(1, 0);
		glm::vec2 p4(-1, 0);
		std::vector<glm::vec2> tab;
		tab.push_back(p1);
		tab.push_back(p2);
		tab.push_back(p3);
		tab.push_back(p4);
		Utils::triangulation * t = incremental_triangulation(tab);

		std::cout << "Number of edge : " << t->edge_list.size() << " - Number of triangle : " << t->triangle_list.size() << std::endl;
		for (int i = 0; i < t->edge_list.size(); i++) {
			std::cout << "Edge " << i << " : " << (t->edge_list[i]->s1.x) << "," << (t->edge_list[i]->s1.y) << " - " << (t->edge_list[i]->s2.x) << "," << (t->edge_list[i]->s2.y) << std::endl;
		}

		Utils::edge_flipping(t->edge_list);

		std::cout << "Number of edge : " << t->edge_list.size() << " - Number of triangle : " << t->triangle_list.size() << std::endl;
		for (int i = 0; i < t->edge_list.size(); i++) {
			std::cout << "Edge " << i << " : " << (t->edge_list[i]->s1.x) << "," << (t->edge_list[i]->s1.y) << " - " << (t->edge_list[i]->s2.x) << "," << (t->edge_list[i]->s2.y) << std::endl;
		}
	}

	void test_circle()
	{
		glm::vec2 a = glm::vec2(0, 1);
		glm::vec2 b = glm::vec2(3, 2);
		glm::vec2 c = glm::vec2(6, 1);

		/*glm::vec2 a = glm::vec2(0, 0);
		glm::vec2 b = glm::vec2(0.5f, 0.5f);
		glm::vec2 c = glm::vec2(-2,2);*/

		std::cout << Utils::get_circumscribed_circle(new Utils::triangle(new Utils::edge(a, b), new Utils::edge(b, c), new Utils::edge(c, a))).c.x << std::endl
			<< Utils::get_circumscribed_circle(new Utils::triangle(new Utils::edge(a, b), new Utils::edge(b, c), new Utils::edge(c, a))).c.y << std::endl;
	}

	MATHLIB_API void test() {
		/*std::cout << "test du barycentre : " << std::boolalpha << test_barycenter() << std::endl;
		std::cout << "test du produit scalaire : " << std::boolalpha << test_scalar_product() << std::endl;
		std::cout << "test de la norme : " << std::boolalpha << test_norm() << std::endl;
		std::cout << "test du get vector from points : " << std::boolalpha << test_vector_from_points() << std::endl;
		std::cout << "test du oriented angle : " << std::boolalpha << test_oriented_angle() << std::endl;
		std::cout << "test du oriented angle 2PI: " << std::boolalpha << test_oriented_angle_2PI() << std::endl;
		std::cout << "test du graham sort : " << std::boolalpha << test_graham_sort() << std::endl;
		std::cout << "test de l'incrementation triangulaire pour 2 points : " << std::boolalpha << test_incremental_triangulation_size2() << std::endl;*/
		test_circle();
		//test_triangulation();
		test_flipping();
	}
#pragma endregion
}