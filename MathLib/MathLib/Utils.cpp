#include "Utils.h"
#include <math.h>
#include <stack>
#include <array>
#include "MathLib.h"
#include <iostream>
#include <ostream>
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
		angle += 2.0 * PI;
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
	return abs(a.x / b.x - a.y / b.y) < 0.001f;
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

	while (!ac.empty())
	{
		edge * tmp_edge = ac.top();
		ac.pop();

		if (tmp_edge->t2 != nullptr && !check_delaunay_crit(tmp_edge)) {
			edge * a1, *a2, *a3, *a4;
			glm::vec2 s1, s2, s3, s4;
			s1 = tmp_edge->s1;
			s2 = tmp_edge->s2;
			s3 = Utils::get_third_vertex(tmp_edge->t2, tmp_edge);
			s4 = Utils::get_third_vertex(tmp_edge->t1, tmp_edge);

			//recherche des aretes de t1
			if (tmp_edge->t1->a1 == tmp_edge) {
				if (tmp_edge->t1->a2->s1 == s1 || tmp_edge->t1->a2->s2 == s1) {
					a1 = tmp_edge->t1->a2;
					a4 = tmp_edge->t1->a3;
				}
				else {
					a4 = tmp_edge->t1->a2;
					a1 = tmp_edge->t1->a3;
				}
			}
			if (tmp_edge->t1->a2 == tmp_edge) {
				if (tmp_edge->t1->a1->s1 == s1 || tmp_edge->t1->a1->s2 == s1) {
					a1 = tmp_edge->t1->a1;
					a4 = tmp_edge->t1->a3;
				}
				else {
					a4 = tmp_edge->t1->a1;
					a1 = tmp_edge->t1->a3;
				}
			}
			if (tmp_edge->t1->a3 == tmp_edge) {
				if (tmp_edge->t1->a1->s1 == s1 || tmp_edge->t1->a1->s2 == s1) {
					a1 = tmp_edge->t1->a1;
					a4 = tmp_edge->t1->a2;
				}
				else {
					a4 = tmp_edge->t1->a1;
					a1 = tmp_edge->t1->a2;
				}
			}
			//recherche des aretes de t2
			if (tmp_edge->t2->a1 == tmp_edge) {
				if (tmp_edge->t2->a2->s1 == s1 || tmp_edge->t2->a2->s2 == s1) {
					a2 = tmp_edge->t2->a2;
					a3 = tmp_edge->t2->a3;
				}
				else {
					a3 = tmp_edge->t2->a2;
					a2 = tmp_edge->t2->a3;
				}
			}
			if (tmp_edge->t2->a2 == tmp_edge) {
				if (tmp_edge->t2->a1->s1 == s1 || tmp_edge->t2->a1->s2 == s1) {
					a2 = tmp_edge->t2->a1;
					a3 = tmp_edge->t2->a3;
				}
				else {
					a3 = tmp_edge->t2->a1;
					a2 = tmp_edge->t2->a3;
				}
			}
			if (tmp_edge->t2->a3 == tmp_edge) {
				if (tmp_edge->t2->a1->s1 == s1 || tmp_edge->t2->a1->s2 == s1) {
					a2 = tmp_edge->t2->a1;
					a3 = tmp_edge->t2->a2;
				}
				else {
					a3 = tmp_edge->t2->a1;
					a2 = tmp_edge->t2->a2;
				}
			}

			//mise a jour des triangle de a2 et a4
			if (Utils::get_third_vertex(a2->t1, a2) == s2) {
				a2->t1 = tmp_edge->t1;
			}
			else {
				a2->t2 = tmp_edge->t1;
			}
			if (Utils::get_third_vertex(a4->t1, a4) == s1) {
				a4->t1 = tmp_edge->t2;
			}
			else {
				a4->t2 = tmp_edge->t2;
			}

			tmp_edge->s1 = s3;
			tmp_edge->s2 = s4;

			tmp_edge->t1->a1 = a1;
			tmp_edge->t1->a2 = a2;
			tmp_edge->t1->a3 = tmp_edge;

			tmp_edge->t2->a1 = a3;
			tmp_edge->t2->a2 = a4;
			tmp_edge->t2->a3 = tmp_edge;

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
	Utils::cercle c1 = get_circumscribed_circle(t1);
	Utils::cercle c2 = get_circumscribed_circle(t2);

	if (c1.r == 0.0f && c2.r == 0.0f) return false;

	glm::vec2 vector1 = Utils::get_vector_from_points(c2.c, Utils::get_third_vertex(t1 , a));
	glm::vec2 vector2 = Utils::get_vector_from_points(c1.c, Utils::get_third_vertex(t2, a));
	return Utils::norm(vector1) > c2.r && Utils::norm(vector2) > c1.r;
}

bool Utils::is_edge_visible(glm::vec2 point, edge * edge, std::vector<Utils::edge*> edge_list)
{
	//calcul du vecteur correspondant au edge
	glm::vec2 edge_vector = glm::vec2(edge->s2.x - edge->s1.x, edge->s2.y - edge->s1.y);

	//trouver l'equation de la droite du edge ax+by+c=0
	float a, b, c;
	if (edge_vector.x < 0.001 && edge_vector.x > -0.001) {
		a = 1; b = 0; c = -edge->s2.x;
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
	for (auto checking_edge : edge_list) {
		float triangle_point_in_equation = a * checking_edge->s1.x + b * checking_edge->s1.y + c;
		if (triangle_point_in_equation > 0.0001 || triangle_point_in_equation < -0.0001)
			return !(Utils::sign(triangle_point_in_equation) == sign(point_in_equation));
		triangle_point_in_equation = a * checking_edge->s2.x + b * checking_edge->s2.y + c;
		if (triangle_point_in_equation > 0.0001 || triangle_point_in_equation < -0.0001)
			return !(Utils::sign(triangle_point_in_equation) == sign(point_in_equation));
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
	//premier milieu
	glm::vec2 a1s1 = triangle->a1->s1;
	glm::vec2 a1s2 = triangle->a1->s2;
	glm::vec2 middle1 = glm::vec2((a1s1.x + a1s2.x) / 2, (a1s1.y + a1s2.y) / 2);
	float p1, m1, x1;
	bool straight1 = false;
	if (abs(a1s2.y - a1s1.y) > 0.001) {//cas y = mx+p
		m1 = -(a1s2.x - a1s1.x) / (a1s2.y - a1s1.y); //TODO check!=milieu
		p1 = middle1.y - m1*middle1.x;
		// on a -m1 x + y - p1 = 0
	}
	else {//cas x=c
		straight1 = true;
		x1 = middle1.x;
	}

	//second milieu
	glm::vec2 a2s1 = triangle->a2->s1;
	glm::vec2 a2s2 = triangle->a2->s2;
	glm::vec2 middle2 = glm::vec2((a2s1.x + a2s2.x) / 2, (a2s1.y + a2s2.y) / 2);
	float p2, m2, x2;
	bool straight2 = false;
	if (abs(a2s2.y - a2s1.y) > 0.001) {
		m2 = -(a2s2.x - a2s1.x) / (a2s2.y - a2s1.y); //TODO check!=0
		p2 = middle2.y - m2*middle2.x;
		// on a -m2 x + y - p2 = 0
	}
	else {//cas x=c
		straight2 = true;
		x2 = middle2.x;
	}

	//resolution de l'intersection
	glm::vec2 center;
	if (straight1) {
		if (straight2) {
			//std::cerr << "triangle plat" << std::endl;
			return cercle();
		}
		center.x = x1;
		center.y = m2 * x1 + p2;
	}

	if (straight2) {
		center.x = x2;
		center.y = m1 * x2 + p1;
	}

	// { -m1 x + y = p1
	// { -m2 x + y = p2
	if (!straight1 && !straight2) {
		float det = m2 - m1;
		if (det == 0) {
			//std::cerr << "triangle plat" << std::endl;
			return cercle();
		}
		center.x = (p2 - p1) / -det;
		center.y = (p1*m2 - p2 * m1) / det;
	}
	float radius = Utils::norm(Utils::get_vector_from_points(center, a1s1));



	return cercle(center, radius);
}

std::vector<Utils::region*> Utils::get_voronoi_regions(std::vector<glm::vec2> points, std::vector<Utils::edge*> edge_list)
{
	std::vector<Utils::region*> regions;
	for (glm::vec2 point : points) {
		Utils::region* region = new Utils::region(point);
		std::vector<Utils::edge*> linked_edges;
		for (Utils::edge* edge : edge_list) {
			if (point == edge->s1 || point == edge->s2) {
				linked_edges.push_back(edge);
			}
		}
		for (Utils::edge* edge : linked_edges) {
			region->dual_edge_list.push_back(Utils::get_dual_edge(edge));
		}
		regions.push_back(region);
	}
	return regions;
}

glm::vec2 Utils::get_edge_middle(edge * edge)
{
	return glm::vec2((edge->s1.x + edge->s2.x) / 2, (edge->s1.y + edge->s2.y) / 2);
}

glm::vec2 Utils::get_third_vertex(Utils::triangle* triangle, Utils::edge* edge) {
	if (triangle->a1 == edge) {
		if (triangle->a2->s1 == edge->s1 || triangle->a2->s1 == edge->s2) {
			return triangle->a2->s2;
		}
		else return triangle->a2->s1;
	}
	else {
		if (triangle->a1->s1 == edge->s1 || triangle->a1->s1 == edge->s2) {
			return triangle->a1->s2;
		}
		else return triangle->a1->s1;
	}
}

glm::vec2 Utils::get_height_foot(Utils::triangle* triangle, Utils::edge* edge) {
	glm::vec2 third_vertex = Utils::get_third_vertex(triangle, edge);
	glm::vec2 A = Utils::get_third_vertex(triangle, triangle->a1);
	glm::vec2 B = Utils::get_third_vertex(triangle, triangle->a2);
	glm::vec2 C = Utils::get_third_vertex(triangle, triangle->a3);
	if (third_vertex == A) {
		return Utils::get_height_foot(A, B, C);
	}
	if (third_vertex == B) {
		return Utils::get_height_foot(B, A, C);
	}
	if (third_vertex == C) {
		return Utils::get_height_foot(C, B, A);
	}
	std::cerr << "probleme : get_height_foot, edge n'est pas dans triangle" << std::endl;
	return glm::vec2();
}

glm::vec2 Utils::get_height_foot(glm::vec2 A, glm::vec2 B, glm::vec2 C) {
	//BC : -m1 x + y = p1
	float p1, m1, x1;
	bool straight1 = false;
	if (abs(C.x - B.x) <= 0.001) {
		x1 = C.x;
		straight1 = true;
	}
	else {
		m1 = (C.y - B.y) / (C.x - B.x);
		p1 = -m1 * B.x + B.y;
	}

	//AH : -1/m1 x + y = p2
	float p2, m2, x2;
	bool straight2 = false;
	if (abs(m1) <= 0.001) {
		x2 = A.x;
		straight2 = true;
	}
	else {
		m2 = 1 / m1;
		p2 = -m2 * A.x + A.y;
	}
	if (straight1) return glm::vec2(x1, A.y);
	if (straight2) return glm::vec2(x2, B.y);
	float det = m2 - m1;
	return glm::vec2((p2 - p1) / det, (p1*m2 - p2 * m1) / det);
}

Utils::edge* Utils::get_dual_edge(Utils::edge * edge)
{
	if (edge->t2 == nullptr) {
		//cas pourri, calculer le centre du triangle, le milieu du edge et la hauteur partant du sommet opposé au edge
		//faire partir du centre avec en vecteur directeur la hauteur la droite vers l'infini
		glm::vec2 center = get_circumscribed_circle(edge->t1).c;
		glm::vec2 middle = get_edge_middle(edge);
		//recherche du 3e sommet
		glm::vec2 third_vertex = get_third_vertex(edge->t1, edge);
		glm::vec2 height_from_third_vertex = Utils::get_vector_from_points(third_vertex, Utils::get_height_foot(edge->t1, edge));
		glm::vec2 far_point = glm::vec2(1000 * (third_vertex.x + height_from_third_vertex.x), 1000 * (third_vertex.y + height_from_third_vertex.y));
		if (!Utils::is_colinear(Utils::edge(center, middle), Utils::edge(center, far_point))) {
			std::cerr << "erreur dans le calcul de la hauteur ou de la demi-droite vers l'exterieur" << std::endl;
		}
		return new Utils::edge(center, far_point);
	}
	else {
		glm::vec2 center1 = get_circumscribed_circle(edge->t1).c;
		glm::vec2 center2 = get_circumscribed_circle(edge->t2).c;
		return new Utils::edge(center1, center2);
	}
}

int Utils::find(std::vector<edge*> edge_list, edge* edge)
{
	for (int i = 0; i < edge_list.size(); i++) {
		bool s1x = abs(edge_list[i]->s1.x - edge->s1.x) < 0.001;
		bool s2x = abs(edge_list[i]->s2.x - edge->s2.x) < 0.001;
		bool s1y = abs(edge_list[i]->s1.y - edge->s1.y) < 0.001;
		bool s2y = abs(edge_list[i]->s2.y - edge->s2.y) < 0.001;
		if ( s1x && s2x && s1y && s2y) {
			return i;
		}
	}
	return -1;
}
