#pragma once
#ifndef DIFEVA_H
#define DIFEVA_H
#include "ArgumentHandler.h"
class DifficultyEvaluation
{
public:
    DifficultyEvaluation();
    ~DifficultyEvaluation();
    Difficulty Evaluate(int p[][9]);
    void GetPuzzle(int p[][9]);
private:
    int puzzle[9][9];
    // candidate[i][j][num]==1 means num is candidate to puzzle[i][j].
    // candidate[i][j][0]==k means puzzle[i][j] has k candidates.
    int candidate[9][9][10];
    // 唯余法排除：某位置出现某数，则同行/宫/列其他位置该数排除候选
    void UpdateSingle();
    // 宫内排除：宫内某数只出现在某行/列，则该行/列其他位置该数排除候选
    void UpdateCell();
    // 行/列/宫显性数对排除法：
    // 行/列/宫某两个位置只出现两个相同的候选数，则若行列内则排除同行列的其余位置这两个数的候选
    // 若这两个位置同宫，则还可排除宫内与两个位置同行列的其余位置这两个数的候选
    void UpdateNakedPair();
    // 行/列/宫隐性数对排除法：
    // 某两个数只出现在某行/列/宫的两个位置，则两位置其余候选数排除
    void UpdateHiddenPair();
    // X-Wing排除法：
    // 若某数在某两行/列中只出现在该两行/列的两个相同位置，则构成X-Wing，排除四边形边上所有位置的该数候选
    void UpdateXWing();

    void CandidateDelete(int row, int col, int num);

    void PuzzleInit(int p[][9]);

    bool IsFinished();
    void Fill(int row, int col, int num);
    bool FillNakedSingle();
    bool FillHiddenSingle();
    bool TryToFill(int &updateCount, int &fillCount1, int &fillCount2);
};

#endif // ! DIFEVA_H
