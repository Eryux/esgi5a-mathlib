#include "stdafx.h"
#include <iostream>
#include <glm.hpp>
#include <vector>

#include "../MathLib/Utils.h"
#include "../MathLib/MathLib.h"

int main()
{
	//Mathlib::test();
	
	float points[] = { 4.56f, -0.21f, 4.56f, 0.26f, 2.81f, 3.67f, -0.60f, 4.53f, -5.23f, 0.68f, -3.54f, -4.40f, 3.26f, -6.15f, 3.39f, -3.28f, -1.95f, -0.60f, -1.07f, 1.74f, 0.21f, -4.35f, -3.39f, -2.08f, -4.66f, -6.12f, -0.42f, -8.23f, 6.46f, -8.23f, 3.18f, -2.99f, 2.58f, -0.16f, 7.55f, -2.60f, 9.61f, 1.07f, 5.16f, 2.94f, 3.23f, 5.31f, 4.22f, 4.35f, -1.35f, 5.76f, -5.26f, 4.43f, -4.84f, 0.31f, -2.89f, 2.55f, 8.44f, 3.13f, 6.43f, 1.88f, 2.68f, 1.59f };
	int size = 58;

	//float points[] = { 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f };
	//int size = 8;

	std::cout << "Marche de Jarvis -------------------" << std::endl;

	int * p_result = new int(0);
	int * r = Mathlib::jarvis_walk(points, size / 2, p_result);

	std::cout << "NB Point : " << *p_result / 2 << std::endl;
	for (int i = 0; i < *p_result; i += 2) {
		std::cout << points[r[i]] << "," << points[r[i + 1]] << std::endl;
	}

	delete p_result;
	delete r;

	std::cout << "Graham scan -------------------" << std::endl;

	p_result = new int(0);
	r = Mathlib::graham_scan(points, size / 2, p_result);

	std::cout << "NB Point : " << *p_result << std::endl;
	for (int i = 0; i < *p_result; i++) {
		std::cout << points[r[i] * 2] << "," << points[r[i] * 2 + 1] << std::endl;
	}

	delete p_result;
	delete r;

	std::cout << "Triangulation -------------------" << std::endl;

	p_result = new int(0);
	float * rt = Mathlib::triangulate(points, size / 2, p_result);

	std::cout << "NB Point : " << *p_result / 2 << std::endl;
	for (int i = 0; i < *p_result; i += 6) {
		std::cout << "Triangle " << i / 6 << " : ";
		std::cout << rt[i] << "," << rt[i + 1] << " - ";
		std::cout << rt[i + 2] << "," << rt[i + 3] << " - ";
		std::cout << rt[i + 4] << "," << rt[i + 5] << std::endl;
	}

	/*std::cout << "Triangulation Delaunay -------------------" << std::endl;

	p_result = new int(0);
	rt = Mathlib::triangulate_delaunay(points, size / 2, p_result);

	std::cout << "NB Point : " << *p_result / 2 << std::endl;
	for (int i = 0; i < *p_result; i += 6) {
		std::cout << "Triangle " << i / 6 << " : ";
		std::cout << rt[i] << "," << rt[i + 1] << " - ";
		std::cout << rt[i + 2] << "," << rt[i + 3] << " - ";
		std::cout << rt[i + 4] << "," << rt[i + 5] << std::endl;
	}*/

	std::cout << "FIN -------------------" << std::endl;
	
	int quit = 0;
	std::cin >> quit;

    return 0;
}


