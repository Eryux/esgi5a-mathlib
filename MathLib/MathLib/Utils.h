#pragma once
#include <glm.hpp>

namespace Utils {
	//get the jarvis angle : arccos(pkpj.director_vec/||pkpj||.||director_vec||
	float jarvis_angle(glm::vec2 director_vec, glm::vec2 pk_pj);
	//x1.x2 + y1.y2
	float scalar_product(glm::vec2 vector1, glm::vec2 vector2);
	//sqrt(vector.vector)
	float norm(glm::vec2 vector);
	//Returns the vector point1point2
	glm::vec2 get_vector_from_points(glm::vec2 point1, glm::vec2 point2);
}