#pragma once
class SdkBuffer
{
private:
	/*
	1.All elements are stored in a row with a char array.There will be transformations between 
	char and int(eg. buffer[i]=num+'0')
	*/
	char *buffer;
	unsigned int size;
	unsigned int capacity;
public:
	/*
	1.This class is used to store boards.
	2.Use Pop and Fill to add and delete board, notice it is a stack structure which means 
	the order you push and the order you pop is opposite.
	3.size is the number of boards it has stored , capacity is the ability to store
	4.Use Get to go through it ,but you can change it only with the last element(Push or Pop)
	5.There are 2 forms of array used in this class:int [9][9] and int [][81],the second format is 
	only used in ToArray function.
	6.size is the number of elements it already holds,capacity is the ability it can holds
	*/
	SdkBuffer(unsigned int capacity);
	unsigned int GetSize();
	unsigned int GetCapacity();
	const char* ToString();
	void InitByArray(int pIntArray[][81],int size);
	void ToArray(int pIntArray[][81]);
	bool IsValid(int board[9][9]);
	void Pop(int board[81]);
	bool Fill(int board[81]);
	void Pop(int board[][9]);
	bool Fill(int board[][9]);
	void Get(int index,int board[][9]);
	void Set(int index, int board[][9]);
	void Clear();
	~SdkBuffer();
};

