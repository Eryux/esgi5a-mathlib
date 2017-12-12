#pragma once
#include <glm.hpp>
#include <vector>
#include <list>


namespace Utils {
	struct triangle;
	struct edge {
		edge(glm::vec2 s1, glm::vec2 s2) { this->s1 = s1; this->s2 = s2; };
		glm::vec2 s1; 
		glm::vec2 s2;
		triangle* t1 = nullptr;
		triangle* t2 = nullptr;
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
		cercle(glm::vec2 c, float r) { this->c = c; this->r = r; };
		cercle() {};
		float r;
		glm::vec2 c;
	};
	
	struct region {
		region(glm::vec2 vertex) { this->vertex = vertex; }
		glm::vec2 vertex;
		std::vector<edge*> dual_edge_list;
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
	void edge_flipping(std::vector<edge*> edges);
	bool check_delaunay_crit(edge * a);
	// Cercle circonscrit
	cercle get_circumscribed_circle(triangle * triangle);
	//check si une arête est visible par un point
	bool is_edge_visible(glm::vec2 point, edge* edge1, std::vector<edge*> edge_list);
	//retourne la liste des arêtes correspondant à mon nuage de points
	std::list<edge*> get_convex_envelope(std::vector<edge*> edge_list);
	//convertit un vector<glm::vec2> en point*
	float* convert_from_vector(std::vector<glm::vec2> points);
	//convertit un vector<glm::vec*> en point*
	//float* convert_from_triangulation(triangulation triangulation);
	// renvoie le cercle circonscrit au triangle
	cercle get_circumscribed_circle(triangle* triangle);
	//calcule les regions du diagramme de voronoï 2D
	std::vector<region*> get_voronoi_regions(std::vector<glm::vec2> points, std::vector<edge*> edge_list);
	//sommet opposé a une arete dans un triangle
	glm::vec2 get_third_vertex(triangle* triangle, edge* edge);
	//milieu d'une arete
	glm::vec2 get_edge_middle(edge* edge);
	//retourne le pied de la hauteur dans edge
	glm::vec2 get_height_foot(Utils::triangle* triangle, Utils::edge* edge);
	//retourne le pied de la hauteur issue de A dans le triangle ABC
	glm::vec2 get_height_foot(glm::vec2 A, glm::vec2 B, glm::vec2 C);
	//calcule l'arête duale A* de l'arête en paramètre
	edge* get_dual_edge(edge* edge);
	//retourne l'index de edge dans edge_list -1 si pas trouvé
	int find(std::vector<edge*> edge_list, edge* edge);
}