#include "Utils.h"
#include <math.h>
#include <array>
#include "MathLib.h";
#define PI 3.14159265359

float Utils::sign(float x)
{
	if (x < 0) return -1.0f;
	return 1.0f;
}

float Utils::jarvis_angle(glm::vec2 director_vec, glm::vec2 pk_pj)
{
	return acos(scalar_product(director_vec, pk_pj)/(norm(director_vec)*norm(pk_pj)));
}

float Utils::oriented_angle(glm::vec2 vector1, glm::vec2 vector2)
{
	float determinant = vector1.x * vector2.y - vector2.x * vector1.y;
	float signe = determinant / abs(determinant);
	return signe * Utils::jarvis_angle(vector1, vector2);
}

float Utils::oriented_angle_2PI(glm::vec2 vector1, glm::vec2 vector2)
{
	float angle = oriented_angle(vector1, vector2);
	if (angle < 0) {
		angle += 2 * PI;
	}
	return angle;
}

float Utils::scalar_product(glm::vec2 vector1, glm::vec2 vector2) {
	return vector1.x*vector2.x + vector1.y*vector2.y;
}

float Utils::norm(glm::vec2 vector) {
	return sqrt(scalar_product(vector, vector));
}

glm::vec2 Utils::get_vector_from_points(glm::vec2 point1, glm::vec2 point2)
{
	return glm::vec2(point2.x - point1.x, point2.y - point1.y);
}

glm::vec2 Utils::get_barycenter(std::vector<glm::vec2> point_list)
{
	float x_sum = 0, y_sum = 0;
	int size = point_list.size();
	for (glm::vec2 point : point_list) {
		x_sum += point.x;
		y_sum += point.y;
	}
	return glm::vec2(x_sum/size, y_sum/size);
}

std::list<int> Utils::graham_sort(glm::vec2 bary, std::vector<glm::vec2> points)
{
	std::vector<glm::vec2*> tmp; for (int i = 0; i < points.size(); tmp.push_back(&points[i]), ++i);
	std::list<int> result_list;

	int minvec_index;
	float minangle;

	do {
		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[i] == nullptr) continue;
			minvec_index = i;
			minangle = oriented_angle_2PI(glm::vec2(1, 0), get_vector_from_points(bary, points[i]));
			break;
		}

		for (int i = 0; i < tmp.size(); i++)
		{
			if (tmp[i] == nullptr) continue;

			float angle = oriented_angle_2PI(glm::vec2(1, 0), get_vector_from_points(bary, points[i]));
			if (angle < minangle) {
				minangle = angle;
				minvec_index = i;
			}
		}
		tmp[minvec_index] = nullptr;
		result_list.push_back(minvec_index);
	} while (result_list.size() != points.size());

	//structure de comparaison sur l'angle entre Ox et le vecteur form� par bary et le point courant
	/*struct Comparator{
		Comparator(glm::vec2 barycenter) { this->barycenter = barycenter; }
		glm::vec2 barycenter;
		bool operator()(glm::vec2 a, glm::vec2 b) const
		{
			return oriented_angle_2PI(glm::vec2(1, 0), get_vector_from_points(barycenter, a)) < oriented_angle_2PI(glm::vec2(1, 0), get_vector_from_points(barycenter, b));
		}
	} ;

	//On utilise une structure d'objet pour pouvoir passer bary au comparateur en l'instanciant avec sa valeur
	//https://stackoverflow.com/questions/4066576/passing-a-parameter-to-a-comparison-function
	std::sort(points.begin(), points.end(), Comparator(bary));

	for (int i = 0; i < points.size(); i++) {
		result_list.push_back(i);
	}*/

	return result_list;
}

bool Utils::is_colinear(glm::vec2 a, glm::vec2 b) {
	if (a.x == 0.0f || b.x == 0.0f) return (b.x == 0.0f && a.x == 0.0f);
	if (a.y == 0.0f || b.y == 0.0f) return (b.y == 0.0f && a.y == 0.0f);
	return a.x / b.x - a.y / b.y < 0.001f;
}

std::vector<Utils::edge*> Utils::get_visible_edges(glm::vec2 point, std::list<edge*> convex_envelope, std::vector<glm::vec2*> triangulation) {
	std::vector<Utils::edge*> visible_edges;
	for (auto edge : convex_envelope) {
		if (is_edge_visible(point, edge, triangulation)) {
			visible_edges.push_back(edge);
		}
	}
	return visible_edges;
}

bool Utils::is_edge_visible(glm::vec2 point, edge * edge, std::vector<glm::vec2*> triangulation)
{
	//calcul du vecteur correspondant au edge
	glm::vec2 edge_vector = glm::vec2(edge->s2.x - edge->s2.x, edge->s2.y - edge->s2.y);

	//trouver l'equation de la droite du edge
	float a, b, c;
	if (edge_vector.x < 0.001 && edge_vector.x > -0.001) {
		a = 1; b = 0; c = -edge->s2.y;
	}
	else {
		a = edge_vector.y / edge_vector.x;
		b = -1; 
		c = edge->s1.y - (a * edge->s1.x);
	}
	//si point est sur la droite (ou presque, il voit pas edge)
	float point_in_equation = a * point.x + b * point.y + c;
	if (point_in_equation < 0.001 && point_in_equation > -0.001) {
		return false;
	}
	// sinon parcours la liste des points de ta triangulation et check pour le premier point qui
	// ne soit pas sur la droite de l'edge si le signe de ce point appliqu� a la droite est diff�rent
	// du signe de point appliqu� a la droite alors ils sont pas du meme cot� et point voit egde
	for (auto triangle : triangulation) {
		for (int i = 0; i < 3; i++) {
			float triangle_point_in_equation = a * triangle[i].x + b * triangle[i].y + c;
			if (triangle_point_in_equation > 0.0001 || triangle_point_in_equation < -0.0001) {
				if (Utils::sign(triangle_point_in_equation) == sign(point_in_equation)) {
					return false;
				}
				return true;
			}
		}
	}
	return false;
}

std::list<Utils::edge*> Utils::get_convex_envelope(float * points, int nb_points)
{
	std::list<Utils::edge*> convex_envelope;
	int* out_size;
	int * indexes = Mathlib::jarvis_walk(points, nb_points, out_size);
	for (int i = 0; i < *out_size; i += 4) {
		Utils::edge* edge = new Utils::edge();
		edge->s1 = glm::vec2(points[indexes[i]], points[indexes[i + 1]]);
		edge->s2 = glm::vec2(points[indexes[i + 2]], points[indexes[i + 3]]);
		convex_envelope.push_back(edge);
	}
	return convex_envelope;
}

float * Utils::convert_from_vector(std::vector<glm::vec2> points)
{
	float* out_point = new float[points.size() * 2]();
	for (int i = 0; i < points.size(); i++) {
		out_point[2 * i] = points[i].x;
		out_point[2 * i + 1] = points[i].y;
	}
	return out_point;
}

float * Utils::convert_from_triangulation(std::vector<glm::vec2*> triangulation)
{
	float* out_point = new float[triangulation.size() * 2 * 3]();
	for (int i = 0; i < triangulation.size(); i++) {
		out_point[2 * i] = triangulation[i][0].x;
		out_point[2 * i + 1] = triangulation[i][0].y;
		out_point[2 * (i + 1)] = triangulation[i][1].x;
		out_point[2 * (i + 1) + 1] = triangulation[i][1].y;
		out_point[2 * (i + 2)] = triangulation[i][2].x;
		out_point[2 * (i + 2) + 1] = triangulation[i][2].y;
	}
	return out_point;
}
