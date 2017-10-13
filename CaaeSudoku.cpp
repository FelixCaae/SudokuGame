#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "FileHandler.h"
#include "ArgumentHandler.h"
#include "Table.h"
#include "SdkBuffer.h"
#include "Core.h"
using namespace std;
int main(int argc, char**args)
{
#ifdef DEBUG
    clock_t start = clock();
#endif // DEBUG
	ArgumentHandler ah;
    ah.ParseInput(argc, args);
    State st = ah.GetState();
    if (st == State::INV)
    {
        return 1;
    }
	FileHandler fh;
    Table tb;
	try {
		if (st == State::GEN)
		{
			do {
				unsigned int count = ah.GetCount();
				/*if (count > MaxCounts)
				{
					cout << "Count is too big" << endl;
					break;
				}*/
				if (!fh.Open("sudoku.txt", "w")) {
					cout << "File IO error!" << endl;
					break;
				}
				tb.GenerateRandomly(count, &fh);
			} while (false);
		}
		else if (st == State::SOV)
		{
			FileHandler dst;
			do {
				if (dst.Open("sudoku.txt", "w") && fh.Open(ah.GetPathName(), "r"))
				{
					tb.Solve(&fh, &dst);
				}
				else
				{
					cout << "Error happend when trying to read puzzle file" << endl;
				}
			} while (false);
			dst.Close();
		}
		else if (st == State::GEG_R || st == State::GEG_U || st == State::GEG_RU)
		{
			bool unique = false;
			unsigned int lower = 1, upper = 1;
			if (st == State::GEG_U || st == State::GEG_RU)
			{
				unique = true;
			}
			if (st == State::GEG_R || st == State::GEG_RU)
			{
				lower = ah.GetLower();
				upper = ah.GetUpper();
			}
			if (fh.Open(gOutputFilePath, "w"))
			{
				int arrayBuffer[gBufferSize][81];
				SdkBuffer sdb(gBufferSize);
				unsigned int count = ah.GetCount();
				unsigned int number;
				for (int i = 0; i < count; i += gBufferSize)
				{
					number = gBufferSize;
					if (count - i < gBufferSize)
					{
						number = count - i;
					}
					generate(count, lower, upper, unique, arrayBuffer);
					sdb.InitByArray(arrayBuffer,number);
					fh.WriteSdb(&sdb);
				}
			}
			else
			{
				//throw
			}
		}
		else if (st == State::GEG_M)
		{
			Difficulty diff = ah.GetDifficulty();
			if (fh.Open(gOutputFilePath, "w"))
			{
				int arrayBuffer[100][81];
				SdkBuffer sdb(gBufferSize);
				unsigned int count = ah.GetCount();
				unsigned int number;
				for (int i = 0; i < count; i += gBufferSize)
				{
					number = gBufferSize;
					if (count - i < gBufferSize)
					{
						number = count - i;
					}
					generate(number, diff, arrayBuffer);
					sdb.InitByArray(arrayBuffer,number);
					fh.WriteSdb(&sdb);
				}
			}
			else
			{
				//throw 
			}
		}
	}
	catch(exception ex)
	{
		//todo(Zijia):
	}
    fh.Close();
#ifdef  DEBUG
    cout << "elapsed" << (float(clock()) - start) / 1000;
    getchar();
#endif //  DEBUG
    return 0;
}