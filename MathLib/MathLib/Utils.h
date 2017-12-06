#pragma once
#include <glm.hpp>

namespace Utils {
	float jarvis_angle(glm::vec2 director_vec, glm::vec2 pk_pj);
	float scalar_product(glm::vec2 vector1, glm::vec2 vector2);
	float norm(glm::vec2 vector);

}