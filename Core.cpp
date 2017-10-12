#include "stdafx.h"
#include "Core.h"
#include "Table.h"
int sum(int a, int b)
{
	return a + b;
}
/*
void generate(int number, int mode, int result[][])
{
}*/
/*
void generate(int number, int lower, int upper, bool unique, int result[][]);
{
}
*/
bool solve(int puzzle[], int solution[81])
{
	int cells[9][9];
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cells[i][j] = puzzle[i * 9 + j];
		}
	}
	SdkBuffer src(1), answer(1);
	src.Fill(cells);
	Table table;
	table.Solve(&src, &answer);
	answer.Pop(solution);
	if (solution[0] == 0)//no answer 
	{
		return false;
	}
	else
	{
		return true;
	}
}
void generate(int number, int result[][81])
{
	Table table;
	SdkBuffer buffer(gBufferSize);
	for (int i = 0; i < number; i + gBufferSize)
	{
		if (i + gBufferSize > number)
		{
			table.GenerateRandomly(i + gBufferSize - number, &buffer);
		}
		else
		{
			table.GenerateRandomly(gBufferSize, &buffer);
		}
		buffer.ToArray(result + i);
		buffer.Clear();
	}
	
}