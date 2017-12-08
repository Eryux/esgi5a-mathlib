#include "Utils.h"
#include <math.h>
#include <array>
#define PI 3.14159265359

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

std::list<glm::vec2> Utils::triangulate_sort(std::vector<glm::vec2> points) {

	std::list<glm::vec2> result_list;

	//structure de comparaison bas�e sur l'abscisse puis l'ordonn�e
	struct Comparator {
		bool operator()(glm::vec2 a, glm::vec2 b) const
		{
			if (a.x == b.x) return a.y < b.y;
			else return a.x < b.x;
		}
	};

	std::sort(points.begin(), points.end(), Comparator());

	for (int i = 0; i < points.size(); i++) {
		result_list.push_back(points[i]);
	}
	return result_list;
}

std::list<glm::vec2> Utils::graham_sort(glm::vec2 bary, std::vector<glm::vec2> points)
{
	std::list<glm::vec2> result_list;

	//structure de comparaison sur l'angle entre Ox et le vecteur form� par bary et le point courant
	struct Comparator{
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
		result_list.push_back(points[i]);
	}
	return result_list;
}

bool Utils::is_colinear(glm::vec2 a, glm::vec2 b) {
	if (a.x == 0.0f || b.x == 0.0f) return (b.x == 0.0f && a.x == 0.0f);
	if (a.y == 0.0f || b.y == 0.0f) return (b.y == 0.0f && a.y == 0.0f);
	return a.x / b.x - a.y / b.y < 0.001f;
}

std::vector<glm::vec2*> Utils::get_visible_edges(glm::vec2 point, std::vector<glm::vec2*> triangulation) {
	std::vector<glm::vec2*> visible_edges;
	glm::vec2* edge = new glm::vec2[2]();
	visible_edges.push_back(edge);
	return visible_edges;
}