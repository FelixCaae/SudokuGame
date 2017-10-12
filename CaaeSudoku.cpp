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
            tb.GenerateRandomly(count, fh);
        } while (false);
    }
    else if(st == State::SOV)
    {
        FileHandler dst;
        do {
            if (dst.Open("sudoku.txt", "w") && fh.Open(ah.GetPathName(), "r"))
            {
                tb.Solve(fh, dst);
            }
            else
            {
                cout << "Error happend when trying to read puzzle file" << endl;
            }
        } while (false);
        dst.Close();
    }
	else if (st == State::GEG_R)
	{
		if (fh.Open(gOutputFilePath, "w"))
	    {
			
		}
		else
		{
            cout << "Error happend when trying to read puzzle file" << endl;
        }
	}
	else if (st == State::GEG_U)
	{

	}
	else if (st == State::GEG_RU)
	{

	}
	else if (st == State::GEG_M)
	{

	}
	else
	{
		return 1;
	}
    fh.Close();
#ifdef  DEBUG
    cout << "elapsed" << (float(clock()) - start) / 1000;
    getchar();
#endif //  DEBUG
    return 0;
}