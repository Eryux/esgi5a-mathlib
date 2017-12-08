#pragma once
#include <glm.hpp>
#include <vector>
#include <list>


namespace Utils {
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
	//returns the dual-chained list sorted by the angle between Ox and the vector formed by bary and the points array
	std::list<int> graham_sort(glm::vec2 bary, std::vector<glm::vec2> points);
}