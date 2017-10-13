#pragma once
#include "SdkBuffer.h"
class FileHandler
{
private:
	FILE * file;
public:
	FileHandler();
	bool Open(const char *pathname,const char*mode);
	void Close();
	void WriteSdb(SdkBuffer* sdb);
	void ReadSdb(SdkBuffer* sdb);
//	void WriteArray(int array[][81],int size);
	bool HasNext();
	~FileHandler();
};

