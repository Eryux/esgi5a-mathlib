#include "stdafx.h"
#include <iostream>
#include <glm.hpp>
#include <vector>

#include "../MathLib/Utils.h"
#include "../MathLib/MathLib.h"

int main()
{
	Mathlib::test();
	
	/*float points[] = { -4.30f, -3.80f, -3.80f, 3.83f, -0.52f, -0.49f, 1.82f, 3.98f, 2.14f, -3.54f, 4.77f, -4.51f, 6.90f, -4.58f, 8.31f, -4.51f, 9.66f, -5.86f};
	int size = 18;

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

	std::cout << "Triangulation Delaunay -------------------" << std::endl;

	p_result = new int(0);
	rt = Mathlib::triangulate_delaunay(points, size / 2, p_result);

	std::cout << "NB Point : " << *p_result / 2 << std::endl;
	for (int i = 0; i < *p_result; i += 6) {
		std::cout << "Triangle " << i / 6 << " : ";
		std::cout << rt[i] << "," << rt[i + 1] << " - ";
		std::cout << rt[i + 2] << "," << rt[i + 3] << " - ";
		std::cout << rt[i + 4] << "," << rt[i + 5] << std::endl;
	}

	std::cout << "FIN -------------------" << std::endl;*/
	
	int quit = 0;
	std::cin >> quit;

    return 0;
}


