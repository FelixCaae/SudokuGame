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
    // Ψ�෨�ų���ĳλ�ó���ĳ������ͬ��/��/������λ�ø����ų���ѡ
    void UpdateSingle();
    // �����ų�������ĳ��ֻ������ĳ��/�У������/������λ�ø����ų���ѡ
    void UpdateCell();
    // ��/��/�����������ų�����
    // ��/��/��ĳ����λ��ֻ����������ͬ�ĺ�ѡ�����������������ų�ͬ���е�����λ�����������ĺ�ѡ
    // ��������λ��ͬ�����򻹿��ų�����������λ��ͬ���е�����λ�����������ĺ�ѡ
    void UpdateNakedPair();
    // ��/��/�����������ų�����
    // ĳ������ֻ������ĳ��/��/��������λ�ã�����λ�������ѡ���ų�
    void UpdateHiddenPair();
    // X-Wing�ų�����
    // ��ĳ����ĳ����/����ֻ�����ڸ�����/�е�������ͬλ�ã��򹹳�X-Wing���ų��ı��α�������λ�õĸ�����ѡ
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