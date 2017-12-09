#include "Utils.h"
#include <math.h>
#include <stack>
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

std::list<glm::vec2> Utils::triangulate_sort(std::vector<glm::vec2> points)
{
	std::list<glm::vec2> result_list;
	//structure de comparaison basée sur l'abscisse puis l'ordonnée
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

	//structure de comparaison sur l'angle entre Ox et le vecteur formé par bary et le point courant
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

bool Utils::is_colinear(Utils::edge a1, Utils::edge b1) {
	glm::vec2 a = glm::vec2(a1.s2.x - a1.s1.x, a1.s2.y - a1.s1.y);
	glm::vec2 b = glm::vec2(b1.s2.x - b1.s1.x, b1.s2.y - b1.s1.y);
	if (a.x == 0.0f || b.x == 0.0f) return (b.x == 0.0f && a.x == 0.0f);
	if (a.y == 0.0f || b.y == 0.0f) return (b.y == 0.0f && a.y == 0.0f);
	return a.x / b.x - a.y / b.y < 0.001f;
}

std::vector<Utils::edge*> Utils::get_visible_edges(glm::vec2 point, std::list<edge*> convex_envelope, std::vector<Utils::edge*> edge_list) {
	std::vector<Utils::edge*> visible_edges;
	for (Utils::edge* edge : convex_envelope) {
		if (is_edge_visible(point, edge, edge_list)) {
			visible_edges.push_back(edge);
		}
	}
	return visible_edges;
}


void Utils::edge_flipping(std::vector<edge*> edges)
{
	std::stack<edge*> ac;
	for (int i = 0; i < edges.size(); i++) { if (edges[i]->t2 != nullptr) ac.push(edges[i]); }

	while ( ! ac.empty()) 
	{
		edge * tmp_edge = ac.top();
		ac.pop();

		if (!check_delaunay_crit(tmp_edge)) {
			edge * a1, * a2, * a3, * a4;
			a1 = tmp_edge->t1->a2;
			a2 = tmp_edge->t2->a2;
			a3 = tmp_edge->t2->a3;
			a4 = tmp_edge->t1->a3;

			tmp_edge->t2->a2->t1 = tmp_edge->t1;
			tmp_edge->t1->a3->t1 = tmp_edge->t2;

			tmp_edge->t1->a1 = tmp_edge;
			tmp_edge->t1->a2 = a2;
			tmp_edge->t1->a3 = a1;

			tmp_edge->t2->a1 = tmp_edge;
			tmp_edge->t2->a2 = a4;
			tmp_edge->t2->a3 = a3;

			ac.push(a1);
			ac.push(a2);
			ac.push(a3);
			ac.push(a4);
		}
	}
}

bool Utils::check_delaunay_crit(Utils::edge * a)
{
	triangle * t1 = a->t1;
	triangle * t2 = a->t2;
	Utils::cercle c = get_circumscribed_circle(t1);
	glm::vec2 vector1 = Utils::get_vector_from_points(c.c, t1->a2->s2);
	glm::vec2 vector2 = Utils::get_vector_from_points(c.c, t2->a2->s2);
	return Utils::norm(vector1) > c.r && Utils::norm(vector2) > c.r;
}

bool Utils::is_edge_visible(glm::vec2 point, edge * edge, std::vector<Utils::edge*> edge_list)
{
	//calcul du vecteur correspondant au edge
	glm::vec2 edge_vector = glm::vec2(edge->s2.x - edge->s2.x, edge->s2.y - edge->s2.y);

	//trouver l'equation de la droite du edge ax+by+c=0
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
	// ne soit pas sur la droite de l'edge si le signe de ce point appliqué a la droite est différent
	// du signe de point appliqué a la droite alors ils sont pas du meme coté et point voit egde
	for (auto edge : edge_list) {
		for (int i = 0; i < 3; i++) {
			float triangle_point_in_equation = a * edge->s1.x + b * edge->s1.y + c;
			if (triangle_point_in_equation > 0.0001 || triangle_point_in_equation < -0.0001)
				return !Utils::sign(triangle_point_in_equation) == sign(point_in_equation);
		}
	}
	return false;
}

std::list<Utils::edge*> Utils::get_convex_envelope(std::vector<Utils::edge*> edge_list)
{
	std::list<Utils::edge*> convex_envelope;
	for (Utils::edge* edge: edge_list) {
		if (edge->t2 == nullptr) convex_envelope.push_back(edge);
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

/*float * Utils::convert_from_triangulation(std::vector<glm::vec2*> triangulation)
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
}*/

Utils::cercle Utils::get_circumscribed_circle(Utils::triangle* triangle){
	glm::vec2 a1s1 = triangle->a1->s1;
	glm::vec2 a1s2 = triangle->a1->s2;
	glm::vec2 middle1 = glm::vec2((a1s1.x + a1s2.x) / 2, (triangle->a1->s1.y + triangle->a1->s2.y) / 2);

	glm::vec2 middle2 = glm::vec2((triangle->a2->s1.x + triangle->a2->s2.x) / 2, (triangle->a2->s1.y + triangle->a2->s2.y) / 2);
	return cercle();
}

