#include "stdafx.h"
#include <iostream>
#include <glm.hpp>
#include <vector>

#include "../MathLib/Utils.h"
#include "../MathLib/MathLib.h"

int main()
{
	//Mathlib::test();

	float points[] = { -1.0f, 1.0f, -0.95f, 0.2f, 0.0f, 1.0f, 0.25f, 0.5f, 1.0f, 0.2f, 0.1f, -0.5f, 0.0f, -1.0f };
	
	std::cout << "Marche de Jarvis -------------------" << std::endl;

	int * p_result = new int(0);
	int * r = Mathlib::jarvis_walk(points, 7, p_result);

	std::cout << "NB Point : " << *p_result / 2 << std::endl;
	for (int i = 0; i < *p_result; i += 2) {
		std::cout << points[r[i]] << "," << points[r[i + 1]] << std::endl;
	}

	delete p_result;
	delete r;

	std::cout << "Graham scan -------------------" << std::endl;

	p_result = new int(0);
	r = Mathlib::graham_scan(points, 7, p_result);

	std::cout << "NB Point : " << *p_result << std::endl;
	for (int i = 0; i < *p_result; i++) {
		std::cout << points[r[i] * 2] << "," << points[r[i] * 2 + 1] << std::endl;
	}

	delete p_result;
	delete r;

	std::cout << "FIN -------------------" << std::endl;

	int quit = 0;
	std::cin >> quit;

    return 0;
}


