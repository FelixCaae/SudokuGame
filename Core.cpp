#include "stdafx.h"
#include "Core.h"
#include "Table.h"
#include "DifficultyEvaluation.h"
int sum(int a, int b)
{
	return a + b;
}
void generate(int number, int mode, int result[][81])
{
	Table table;
	SdkBuffer board(1);
	int buffer[9][9];
	Difficulty diff;
	DifficultyEvaluation diev;
	for (int i = 0; i < number; i++)
	{
		table.GenerateRandomly(1, &board);
		table.DigRandomCells(&board, gRange[mode-1][0], gRange[mode-1][1], 1);
		board.Get(0, buffer);
		diff=diev.Evaluate(buffer);
		if (diff != mode)
		{
			i--;
			board.Clear();
			continue;
		}
		board.ToArray(result + i);
		board.Clear();
	}
}
void generate(int number, int lower, int upper, bool unique, int result[][81])
{
	Table table;
	SdkBuffer *buffer=new SdkBuffer(gBufferSize);
	for (unsigned int i = 0; i < number; i += gBufferSize)
	{
		if (i + gBufferSize > number)
		{
			table.GenerateRandomly(number - i, buffer);
		}
		else
		{
			table.GenerateRandomly(gBufferSize, buffer);
		}
		table.DigRandomCells(buffer, lower, upper, unique);
		buffer->ToArray(result + i);
		buffer->Clear();
	}
}
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
	int result;
	SdkBuffer board(1);
	board.Fill(cells);
	Table table;
	result=table.Solve(&board);
	board.Pop(solution);
	if (result == 0)//no answer 
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
	SdkBuffer* buffer=new SdkBuffer(gBufferSize);
	for (unsigned int i = 0; i < number; i += gBufferSize)
	{
		if (i + gBufferSize > number)
		{
			table.GenerateRandomly(number - i, buffer);
		}
		else
		{
			table.GenerateRandomly(gBufferSize, buffer);
		}
		buffer->ToArray(result + i);
		buffer->Clear();
	}
	
}