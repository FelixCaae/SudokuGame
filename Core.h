#pragma once
#include "stdafx.h"
//#define DLLOUTPUT
#ifdef DLLOUTPUT
_declspec(dllexport) int sum(int a, int b);
_declspec(dllexport) void generate(int number, int mode, int result[][]);
_declspec(dllexport) void generate(int number, int lower, int upper, bool unique, int result[][]);
_declspec(dllexport) bool solve(int puzzle[], int solution[]);
_declspec(dllexport) void generate(int number, int result[][]);
#endif 
