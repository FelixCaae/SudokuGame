#pragma once
#define DEBUG
enum State { GEN, SOV, GEG_M, GEG_R, GEG_U, GEG_RU, INV };
enum Difficulty { UNS, EASY=0, NORMAL=1, HARD=2 };
const unsigned  int gBufferSize = 10000;
const unsigned int gMaxGenAmount = 10000000;
const char* gOutputFilePath = "sudoku.txt";
const int gRange[3][2] = { {30,40},{35,45},{40,55} };
/*
1.gBufferSize is used to accelerate the generating function.
2.10,000,000 is the biggest number of which you can get in one generating
process;
3.gOutputFilePath is the file used to store the all-generated boards or
the solutions to puzzles.
*/
//const int MaxCounts = 1000000;