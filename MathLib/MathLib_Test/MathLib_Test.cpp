#include "stdafx.h"
#include <iostream>

#include "../MathLib/MathLib.h"

int main()
{
	std::cout << "10 ^ 3 = " << Mathlib::classic_pow(10, 3) << std::endl;

	int quit = 0;
	std::cin >> quit;

    return 0;
}

