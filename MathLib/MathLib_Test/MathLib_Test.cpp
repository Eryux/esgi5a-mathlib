#include "stdafx.h"
#include <iostream>

#include "../MathLib/MathLib.h"

int main()
{
	std::cout << "10 ^ 3 = " << Mathlib::classic_pow(10, 3) << std::endl;

	float points[] = { -1.0f, 1.0f, -0.95f, 0.2f, 0.0f, 1.0f, 0.25f, 0.5f, 1.0f, 0.2f, 0.1f, -0.5f, 0.0f, -1.0f };
	
	int * p_result = new int(0);
	int * r = Mathlib::jarvis_walk(points, 7, p_result);
	std::cout << *p_result << std::endl;

	for (int i = 0; i < *p_result / 2; i++) {
		std::cout << points[r[i]] << "," << points[r[i + 1]] << std::endl;
	}

	delete p_result;
	delete r;

	int quit = 0;
	std::cin >> quit;

    return 0;
}

