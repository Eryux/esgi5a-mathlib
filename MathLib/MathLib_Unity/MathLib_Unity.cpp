#include "stdafx.h"
#include <iostream>

typedef int(_cdecl * dll_mathlib_classic_pow_ptr)(int, int);

typedef int * (_cdecl * dll_mathlib_jarvis_walk_ptr)(float *, int, int*);

typedef int * (_cdecl * dll_mathlib_graham_scan_ptr)(float *, int, int*);

HINSTANCE hGetProcIDDL;

extern "C"
{
	__declspec(dllexport) int classic_pow(int, int);

	__declspec(dllexport) int * jarvis_walk(float *, int, int*);

	__declspec(dllexport) int * graham_scan(float *, int, int*);
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

int * jarvis_walk(float * p, int s, int* o) 
{
	LoadDLL();

	dll_mathlib_jarvis_walk_ptr f = (dll_mathlib_jarvis_walk_ptr)GetProcAddress(hGetProcIDDL, "jarvis_walk");
	if (!f) {
		std::cerr << "Could not find function jarvis_walk in MathLib.dll" << std::endl;
		return nullptr;
	}

	int * result = f(p, s, o);
	freeDLL();
	return result;
}

int * graham_scan(float * p, int s, int* o) 
{
	LoadDLL();

	dll_mathlib_graham_scan_ptr f = (dll_mathlib_graham_scan_ptr)GetProcAddress(hGetProcIDDL, "graham_scan");
	if (!f) {
		std::cerr << "Could not find function graham_scan in Mathlib.dll" << std::endl;
		return nullptr;
	}

	int * result = f(p, s, o);
	freeDLL();
	return result;
}