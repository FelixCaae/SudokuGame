#pragma once
#include "stdafx.h"
//#define DLLOUTPUT
_declspec(dllexport) int sum(int a, int b);
_declspec(dllexport) void generate(int number, int mode, int result[][81]);
_declspec(dllexport) void generate(int number, int lower, int upper, bool unique, int result[][81]);
_declspec(dllexport) bool solve(int puzzle[], int solution[]);
_declspec(dllexport) void generate(int number, int result[][81]);

