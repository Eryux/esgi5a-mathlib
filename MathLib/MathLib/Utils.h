#pragma once
#include <glm.hpp>
#include <vector>
#include <list>


namespace Utils {

	struct edge {
		edge(glm::vec2 s1, glm::vec2 s2) { this->s1 = s1; this->s2 = s2; };
		glm::vec2 s1, s2;
		triangle* t1;
		triangle* t2;
	};

	struct triangle {
		triangle(edge* a1, edge* a2, edge* a3) { this->a1 = a1; this->a2 = a2; this->a3 = a3; };
		edge * a1;
		edge * a2;
		edge * a3;
	};

	struct triangulation {
		std::vector<edge*> edge_list;
		std::vector<triangle*> triangle_list;
	};

	struct cercle {
		float r;
		glm::vec2 c;
	};

	float sign(float x);
	//get the jarvis angle : arccos(pkpj.director_vec/||pkpj||.||director_vec||
	float jarvis_angle(glm::vec2 director_vec, glm::vec2 pk_pj);
	//get the oriented angle using determinant and jarvis angle
	float oriented_angle(glm::vec2 vector1, glm::vec2 vector2);
	//get the oriented angle using determinant and jarvis angle between 0 and 2PI
	float oriented_angle_2PI(glm::vec2 vector1, glm::vec2 vector2);
	//x1.x2 + y1.y2
	float scalar_product(glm::vec2 vector1, glm::vec2 vector2);
	//sqrt(vector.vector)
	float norm(glm::vec2 vector);
	//Returns the vector point1point2
	glm::vec2 get_vector_from_points(glm::vec2 point1, glm::vec2 point2);
	//returns the barycenter of an array of points
	glm::vec2 get_barycenter(std::vector<glm::vec2> point_list);
	//returns the dual-chained list sorted by the abscisse and then ordinate
	std::list<glm::vec2> triangulate_sort(std::vector<glm::vec2> points);
	//returns the dual-chained list sorted by the angle between Ox and the vector formed by bary and the points array
	std::list<int> graham_sort(glm::vec2 bary, std::vector<glm::vec2> points);
	//check if two vectors are colinear
	bool is_colinear(edge a, edge b);
	//retourne les aretes visibles par point dans triangulation
	std::vector<edge*> get_visible_edges(glm::vec2 point, std::list<edge*> convex_envelope, std::vector<edge*> edge_list);
	// Flipping d'arete
	void edge_flipping(triangle * t1, triangle * t2);
	bool check_delaunay_crit(edge * a);
	//check si une arête est visible par un point
	bool is_edge_visible(glm::vec2 point, edge* edge, std::vector<Utils::edge*> edge_list);
	//retourne la liste des arêtes correspondant à mon nuage de points
	std::list<edge*> get_convex_envelope(std::vector<edge*> edge_list);
	//convertit un vector<glm::vec2> en point*
	float* convert_from_vector(std::vector<glm::vec2> points);
	//convertit un vector<glm::vec*> en point*
	float* convert_from_triangulation(triangulation triangulation);
	// renvoie le cercle circonscrit au triangle
	cercle get_circumscribed_circle(triangle* triangle);
}