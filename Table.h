#pragma once
#include"SdkBuffer.h"
#include "FileHandler.h"
extern const unsigned int BufferSize;
class Table
{
private:
	int cells[9][9];
public:
	/*
	1.This class is used to generate or solve sudokus in large amount.
	2.Use Generate to generate and it overrides with different destinations.Solve also overrides.
	3.Solve(SdkBuffer*) will modify the input to tell you the result.If an sudoku is not solvable,it will
	remain as it was.The Solve function will return the number of sudokus tha can be solved.
	*/
	Table();
	void Generate(unsigned int total, SdkBuffer* sdb);
	void GenerateRandomly(unsigned int total, SdkBuffer * sdb);
	void Generate(unsigned int total, FileHandler* fh);
	void GenerateRandomly(unsigned int total, FileHandler* fh);
	int Solvable(SdkBuffer* sdb, int index);
	unsigned int Solve(SdkBuffer* pBoard);
	//Modified:Solve(SdkBuffer* src,SdkBuffer* dst)==> Solve(SdkBuffer* pBoard);
	void Solve(FileHandler* src, FileHandler*dst);
	void DigRandomCells(SdkBuffer* pBoards,unsigned int lower,
		unsigned int upper,bool isAnswerUnique);
	~Table();
private:
	/*
	*/
	void setZero();
	void digSpecNum(int[][9],unsigned int num);
	void digSpecNumUniquely(int[][9], unsigned int num);
	unsigned int startSolving(unsigned int maxAnswer,SdkBuffer*pResult);
	void solve(int subt, int num,unsigned int &total,unsigned int &top,SdkBuffer* pResult);
	int* lookUp(int rst, int cst, int num);
};

