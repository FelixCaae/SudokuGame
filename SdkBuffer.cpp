#include "stdafx.h"
#include "SdkBuffer.h"
#include <memory>

SdkBuffer::SdkBuffer(unsigned int capacity)
{
	unsigned int length = 81 * capacity;
	buffer = new char[length];
	memset(buffer, '0', sizeof(char) *length);
	this->capacity = capacity;
	this->size = 0;
}
SdkBuffer::~SdkBuffer()
{
	delete[] buffer;
}
bool SdkBuffer::Fill(int table[][9])
{
	if (size < capacity)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (table[i][j] > 9 || table[i][j] < 0)return false;
				buffer[81 * size + i * 9 + j] = '0' + table[i][j];
			}
		}
		size += 1;
		return true;
	}
	else
		return false;
}
bool SdkBuffer::Fill(int table[81])
{
	if (size < capacity)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (table[i*9+j] > 9 || table[i*9+j] < 0)return false;
				buffer[81 * size + i * 9 + j] = '0' + table[i * 9 + j];
			}
		}
		size += 1;
		return true;
	}
	else
		return false;
}
void SdkBuffer::Pop(int buffer[][9])
{
	if (size > 0)
	{
		size -= 1;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				buffer[i][j] = this->buffer[81 * size + i * 9 + j]-'0';
			}
		}
	}
}
void SdkBuffer::Pop(int buffer[81])
{
	if (size > 0)
	{
		size -= 1;
		for (int i = 0; i < 81; i++)
		{
			buffer[i] = this->buffer[81 * size + i] - '0';
		}
	}
}
void SdkBuffer::Get(int index, int element[][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++) {
			element[i][j] = buffer[index*81+i * 9 + j] - '0';
		}
	}
}
void SdkBuffer::Set(int index, int element[][9])
{
	//what should we do if index is bigger than size? or index is less than zero?
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++) {
			buffer[index * 81 + i * 9 + j] = element[i][j]  - '0';
		}
	}
}
void SdkBuffer::Clear()
{
	size = 0;
}
const char* SdkBuffer::ToString()
{
	return buffer;
}
void SdkBuffer::ToArray(int (*result)[81])
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 81; j++)
		{
			result[i][j] = buffer[i * 81 + j] - '0';
		}
	}
}
unsigned int SdkBuffer::GetSize()
{
	return size;
}
unsigned int SdkBuffer::GetCapacity()
{
	return capacity;
}