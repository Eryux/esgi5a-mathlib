#include "stdafx.h"
#include <iostream>

typedef int(_cdecl * dll_mathlib_classic_pow_ptr)(int, int);

HINSTANCE hGetProcIDDL;

extern "C"
{
	__declspec(dllexport) int classic_pow(int, int);
}

void LoadDLL()
{
	if (!hGetProcIDDL) {
#if _WIN64
		hGetProcIDDL = LoadLibrary(L"..\\MathLib\\x64\\Debug\\MathLib.dll");
#elif _WIN32
		hGetProcIDDL = LoadLibrary(L"..\\MathLib\\Debug\\MathLib.dll");
#endif

		if (!hGetProcIDDL) {
			std::cerr << "Could not load MathLib.dll." << std::endl;
		}
	}
}

void freeDLL()
{
	FreeLibrary(hGetProcIDDL);
	hGetProcIDDL = nullptr;
}

int classic_pow(int a, int p) 
{
	LoadDLL();

	dll_mathlib_classic_pow_ptr f = (dll_mathlib_classic_pow_ptr)GetProcAddress(hGetProcIDDL, "classic_pow");
	if (!f) {
		std::cerr << "Could not find function classic_pow in MathLib.dll" << std::endl;
		return -1;
	}

	int r = f(a, p);
	freeDLL();
	return r;
}