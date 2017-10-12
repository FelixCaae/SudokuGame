#pragma once
#include "CaaeSudoku.h"
class ArgumentHandler
{

public:
	ArgumentHandler();
	void ParseInput(int argc, char **argv);
	State GetState();
	unsigned int GetCount();
	const char * GetPathName();
	Difficulty GetDifficulty();
	unsigned int GetLower();
	unsigned int GetUpper();
	~ArgumentHandler();
public:
	static bool IsDigit(const char *in);
	static bool JudgeR(const char *in);
private:
	State state;
	Difficulty difc;
	unsigned int count;
	char* pathname;
	bool unique;
	int lower;
	int upper;
};