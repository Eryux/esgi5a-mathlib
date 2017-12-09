#include "stdafx.h"
#include <iostream>
#include <glm.hpp>
#include <vector>

#include "../MathLib/Utils.h"
#include "../MathLib/MathLib.h"

int main()
{
	Mathlib::test();
	/*
	float points[] = { -4.02f, 3.78f, -5.70f, -1.96f, 5.85f, -4.30f, 7.20f, 4.63f, 0.40f, 0.27f, 6.84f, -1.92f, 1.32f, -4.81f, -1.97f, -5.87f, -3.22f, 1.12f, 6.58f, -0.09f, 3.14f, 3.24f, 1.68f, -0.86f, 3.47f, -4.88f, -6.11f, -4.55f, -6.98f, 1.74f, -0.99f, 5.54f, 4.10f, 6.20f, 7.57f, 2.29f, 11.01f, -1.33f };
	
	std::cout << "Marche de Jarvis -------------------" << std::endl;

	int * p_result = new int(0);
	int * r = Mathlib::jarvis_walk(points, 38 / 2, p_result);

	std::cout << "NB Point : " << *p_result / 2 << std::endl;
	for (int i = 0; i < *p_result; i += 2) {
		std::cout << points[r[i]] << "," << points[r[i + 1]] << std::endl;
	}

	delete p_result;
	delete r;

	std::cout << "Graham scan -------------------" << std::endl;

	p_result = new int(0);
	r = Mathlib::graham_scan(points, 38 / 2, p_result);

	std::cout << "NB Point : " << *p_result << std::endl;
	for (int i = 0; i < *p_result; i++) {
		std::cout << points[r[i] * 2] << "," << points[r[i] * 2 + 1] << std::endl;
	}

	delete p_result;
	delete r;

	std::cout << "FIN -------------------" << std::endl;
	*/
	int quit = 0;
	std::cin >> quit;

    return 0;
}


