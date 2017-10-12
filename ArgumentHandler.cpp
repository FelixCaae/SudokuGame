#include "stdafx.h"
#include <iostream>
#include "ArgumentHandler.h"
using namespace std;
const int maxN = 10000;
const int minR = 20;
const int maxR = 55;
const int maxCounts = 1000000;
ArgumentHandler::ArgumentHandler()
{
    state = INV;
    count = 0;
    lower = 0;
    upper = 0;
    pathname = NULL;
    unique = false;
    difc = UNS;
}
ArgumentHandler::~ArgumentHandler()
{
}
const char *ArgumentHandler::GetPathName()
{
    return pathname;
}
unsigned int ArgumentHandler::GetCount()
{
    return count;
}
State ArgumentHandler::GetState()
{
    return state;
}
bool ArgumentHandler::IsDigit(const char *in)
{
    bool r = true;
    for (int i = 0; in[i] != '\0'; ++i)
    {
        r &= (in[i] <= '9'&&in[i] >= '0');
    }
    return r;
}
bool ArgumentHandler::JudgeR(const char *in)
{
    bool r = true;
    int count = 0;
    for (int i = 0; in[i] != '\0'; ++i)
    {
        r &= ((in[i] <= '9'&&in[i] >= '0') || (in[i] == '~'));
        if (in[i] == '~')
            count++;
    }
    return r&(count == 1);
}
Difficulty ArgumentHandler::GetDifficulty()
{
    return difc;
}
unsigned int ArgumentHandler::GetLower()
{
    return lower;
}
unsigned int ArgumentHandler::GetUpper()
{
    return upper;
}
void ArgumentHandler::ParseInput(int argc,  char** args)
{
    if (argc < 3)
    {
        cout << "The number of arguments is not correct!" << endl;
        return;
    }
    if (strcmp(args[1], "-c") == 0)
    {
        state = State::GEN;
        try
        {
            if (argc != 3)
            {
                throw invalid_argument("Argument \"-c\" shouldn't be used with other arguments!");
            }
            else if (!IsDigit(args[2]))
            {
                throw invalid_argument("The argument of \"-c\" should be a positive integer!");
            }
            sscanf_s(args[2], "%d", &count);
            if (count > maxCounts||count<=0)
            {
                throw invalid_argument("The argument of \"-c\" should be in range [1,1000000]!");
            }
        }
        catch (invalid_argument err)
        {
            state = State::INV;
            cout << err.what() << endl;
        }
    }
    else if (strcmp(args[1], "-s") == 0)
    {
        state = State::SOV;
        try
        {
            if (argc != 3)
            {
                throw invalid_argument("Argument \"-s\" shouldn't be used with other arguments!");
            }
            pathname = args[2];
        }
        catch (invalid_argument err)
        {
            state = State::INV;
            cout << err.what() << endl;
        }
    }
    else
    {
		state = GEG_M;
        int nUsed = 0;
        int mUsed = 0;
        int rUsed = 0;
        int uUsed = 0;
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(args[i], "-n") == 0)
            {
                nUsed++;
                try
                {
                    if (i + 1 == argc)
                    {
                        throw invalid_argument("Required argument of \"-n\" missing!");
                    }
                    if (!IsDigit(args[i + 1]))
                    {
                        throw invalid_argument("The argument of \"-n\" should be a positive integer!");
                    }
                    sscanf_s(args[i + 1], "%d", &count);
                    if (count > maxN||count<=0)//test int.max+1

                    {
                        throw invalid_argument("The argument of \"-n\" should be in range [1,10000]!");
                    }
                    i++;
                }
                catch (invalid_argument err)
                {
                    cout << err.what() << endl;
                    state = State::INV;
                    break;
                }
            }
            else if (strcmp(args[i], "-m") == 0)
            {
                mUsed++;
                try
                {
                    if (i + 1 == argc)
                    {
                        throw invalid_argument("Required argument of \"-m\" missing!");
                    }
                    if (args[i+1][1]!='\0')
                    {
                        throw invalid_argument("The argument of \"-m\" should be only a digit!");
                    }
                    switch (args[i + 1][0])
                    {
                    case '1':
                        difc = Difficulty::EASY;
                        break;
                    case '2':
                        difc = Difficulty::NORMAL;
                        break;
                    case '3':
                        difc = Difficulty::HARD;
                        break;
                    default:
                        throw invalid_argument("The argument of \"-m\" should be only 1, 2 or 3!");
                        break;
                    }
                    i++;
                }
                catch (invalid_argument err)
                {
                    cout << err.what() << endl;
                    state = State::INV;
                    break;
                }
            }
            else if (strcmp(args[i], "-u") == 0)
            {
                uUsed++;
                unique = true;
            }
            else if (strcmp(args[i], "-r") == 0)
            {
                rUsed++;
                try
                {
                    if (i + 1 == argc)
                    {
                        throw invalid_argument("Required argument of \"-r\" missing!");
                    }
                    if (!JudgeR(args[i + 1]) || (sscanf_s(args[i + 1], "%d~%d", &lower, &upper) != 2))
                    {
                        throw invalid_argument("The argument of \"-r\" should be in format: \
lower~upper, and lower/upper must be a positive integer!");
                    }
                    if (lower < minR || lower > maxR || upper < minR || upper > maxR || lower > upper)
                    {
                        throw invalid_argument("The two arguments of \"-r\" should be in range [20, 55]\
, and the first number must be no more than the second number!");
                    }
                    i++;
                }
                catch (invalid_argument err)
                {
                    cout << err.what() << endl;
                    state = State::INV;
                    break;
                }
            }
            else
            {
                state = State::INV;
                cout << "We only support the arguments as follows: -c, -s, -n, -r, -m, -u! And -c/-s can only be used alone!" << endl;
                break;
            }
        }
        if (state == State::GEG_M)
        {
            if (nUsed > 1 || uUsed > 1 || mUsed > 1 || rUsed > 1)
            {
                cout << "Every argument should be used only once!" << endl;
                state = State::INV;
            }
            else if (mUsed&rUsed || mUsed&uUsed)
            {
                cout << "Argument -m shouldn't be used with -r or -u!" << endl;
                state = State::INV;
            }
            else if (!nUsed || (nUsed & !mUsed & !rUsed & !uUsed))
            {
                cout << "Argument -n must be used at least once, and it should be used together with -m/-r/-u!" << endl;
                state = State::INV;
            }
			else if(rUsed>0 && uUsed>0)
			{ 
				state = GEG_RU;
			}
			else if (rUsed > 0)
			{
				state = GEG_R;
			}
			else  if (uUsed > 0)
			{
				state = GEG_U;
			}
        }

    }
}
