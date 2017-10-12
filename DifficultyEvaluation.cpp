#include "stdafx.h"
#include "DifficultyEvaluation.h"
#include <vector>
#include <utility>
#include <fstream>
using namespace std;
DifficultyEvaluation::DifficultyEvaluation()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            puzzle[i][j] = 0;
            for (int k = 1; k < 10; ++k)
            {
                candidate[i][j][k] = 1;
            }
            candidate[i][j][0] = 9;
        }
    }
}
DifficultyEvaluation::~DifficultyEvaluation(){}

/*
初始化需要评定难度的数独题目；
已填位置的候选数计数置0；
*/
void DifficultyEvaluation::PuzzleInit(int p[][9])
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            puzzle[i][j] = p[i][j];
            if (puzzle[i][j] != 0)
            {
                candidate[i][j][0] = 0;
            }
        }
    }
}

/*
删除(row, col)位置的候选数num；
若该位置已填或本来就没有候选数num，则不做任何操作；
*/
void DifficultyEvaluation::CandidateDelete(int row, int col, int num)
{
    if (puzzle[row][col]==0 && candidate[row][col][num] == 1)
    {
        candidate[row][col][num] = 0;
        candidate[row][col][0]--;
    }
}

/*
唯余法排除（按数独规则排除候选数）
*/
void DifficultyEvaluation::UpdateSingle()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (puzzle[i][j] != 0)
            {
                // update rows and columns
                for (int k = 0; k < 9; ++k)
                {
                    CandidateDelete(i, k, puzzle[i][j]);
                    CandidateDelete(k, j, puzzle[i][j]);
                }
                // update cells
                for (int ci = 3 * (i / 3); ci < 3 * (i / 3) + 3; ++ci)
                {
                    for (int cj = 3 * (j / 3); cj < 3 * (j / 3) + 3; ++cj)
                    {
                        CandidateDelete(ci, cj, puzzle[i][j]);
                    }
                }
            }
        }
    }
}

/*
宫内排除法；
*/
void DifficultyEvaluation::UpdateCell()
{
    // 遍历9个宫
    for (int cellnum = 0; cellnum < 9; ++cellnum)
    {
        // 计算此宫的起始位置坐标
        int rs = 3 * (cellnum / 3);
        int cs = 3 * (cellnum % 3);
        // 1至9遍历，计算num的作为候选数的出现次数
        for (int num = 1; num <= 9; ++num)
        {
            // appear保存num作为候选数出现的位置坐标
            vector<pair<int, int>> appear;
            for (int row = rs; row < rs + 3; ++row)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col]==0 && candidate[row][col][num]==1)
                    {
                        // 找到num出现的位置，保存之
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            if (appear.size() == 3)
            {
                // 若num出现3次并同行或同列，则删除其他宫同行/列的num候选数
                if (appear[0].first == appear[1].first && appear[1].first == appear[2].first)// 判断同行
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        if (col < cs || col > cs + 2)
                        {
                            CandidateDelete(appear[0].first, col, num);
                        }
                    }
                }
                else if (appear[0].second == appear[1].second && appear[1].second == appear[2].second)// 判断同列
                {
                    for (int row = 0; row < 9; ++row)
                    {
                        if (row < rs || row > rs + 2)
                        {
                            CandidateDelete(row, appear[0].second, num);
                        }
                    }
                }
            }
            else if (appear.size() == 2)
            {  
                // 若num出现2次并同行或同列，则删除其他宫同行/列的num候选数
                if (appear[0].first == appear[1].first)// 判断同行
                {
                    for (int col = 0; col < 9; ++col)
                    {
                        if (col < cs || col > cs + 2)
                        {
                            CandidateDelete(appear[0].first, col, num);
                        }
                    }
                }
                else if (appear[0].second == appear[1].second)// 判断同列
                {
                    for (int row = 0; row < 9; ++row)
                    {
                        if (row < rs || row > rs + 2)
                        {
                            CandidateDelete(row, appear[0].second, num);
                        }
                    }
                }
            }
        }
    }
}

/*
显式数对排除法；
*/
void DifficultyEvaluation::UpdateNakedPair()
{
    for (int k = 0; k < 9; ++k)
    {
        // kth row
        // 判断第k行是否存在显式数对
        for (int col = 0; col < 8; ++col)
        {
            if (candidate[k][col][0] != 2)
                continue;
            // 找到候选数数量为2的一个位置
            // 从该位置向后寻找一个和该位置候选数对相同的位置
            for (int ncol = col + 1; ncol < 9; ++ncol)
            {
                if (candidate[k][ncol][0] != 2)
                    continue;
                // 找到另一个候选数数量为2的位置
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    // 判断两个位置的候选数对是否相同
                    r &= candidate[k][col][i] == candidate[k][ncol][i];
                    // n1,n2记录2个候选数
                    if (candidate[k][col][i] == 1)
                    {
                        if (n1 == 0)
                        {
                            n1 = i;
                        }
                        n2 = i;
                    }
                }
                if (r)
                {
                    // 两个位置具有相同的候选数对
                    // update row k: delete n1, n2 except (k,col), (k,ncol)
                    for (int i = 0; i < 9; ++i)
                    {
                        if (i != col && i != ncol)
                        {
                            CandidateDelete(k, i, n1);
                            CandidateDelete(k, i, n2);
                        }
                    }
                }
            }
        }
        // kth column
        // 判断第k列是否存在显式数对（结构同第k行的情形）
        for (int row = 0; row < 8; ++row)
        {
            if (candidate[row][k][0] != 2)
                continue;
            for (int nrow = row + 1; nrow < 9; ++nrow)
            {
                if (candidate[nrow][k][0] != 2)
                    continue;
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    r &= candidate[row][k][i] == candidate[nrow][k][i];
                    if (candidate[row][k][i] == 1)
                    {
                        if (n1 == 0)
                        {
                            n1 = i;
                        }
                        n2 = i;
                    }
                }
                if (r)
                {
                    // update column k: delete n1, n2 except row, nrow
                    for (int i = 0; i < 9; ++i)
                    {
                        if (i != row && i != nrow)
                        {
                            CandidateDelete(i, k, n1);
                            CandidateDelete(i, k, n2);
                        }
                    }
                }
            }
        }
        // kth cell
        // 判断第k宫是否存在显式数对
        for (int cln = 0; cln < 8; ++cln)
        {
            // cln为宫的线性位置0-7
            // 计算cln对应的第k宫的坐标位置
            int row = 3 * (k / 3) + cln / 3;
            int col = 3 * (k % 3) + cln % 3;
            if (candidate[row][col][0] != 2)
                continue;
            // 找到候选数数量为2的一个位置
            // 从该位置向后寻找一个和该位置候选数对相同的位置
            for (int ncln = cln + 1; ncln < 9; ++ncln)
            {
                // 计算ncln对应的第k宫的位置
                int nrow = 3 * (k / 3) + ncln / 3;
                int ncol = 3 * (k % 3) + ncln % 3;
                if (candidate[nrow][ncol][0] != 2)
                    continue;
                // 找到另一个候选数数量为2的位置
                bool r = true;
                int n1 = 0;
                int n2 = 0;
                for (int i = 1; i < 10; ++i)
                {
                    // 判断这两个位置的候选数是否相同
                    r &= candidate[row][col][i] == candidate[nrow][ncol][i];
                    if (candidate[row][col][i] == 1)
                    {
                        if (n1 == 0)
                        {
                            n1 = i;
                        }
                        n2 = i;
                    }
                }
                if (r)
                {
                    // 如果相同则删去宫内其他位置的两数候选
                    // 这两个位置如果同行或同列，之前两个循环已对同行列做处理，无需再做
                    int rs = 3 * (k / 3);
                    int cs = 3 * (k % 3);
                    for (int i = rs; i < rs + 3; ++i)
                    {
                        for (int j = cs; j < cs + 3; ++j)
                        {
                            if ((i != row||j != col) && (i != nrow||j != ncol))
                            {
                                CandidateDelete(i, j, n1);
                                CandidateDelete(i, j, n2);
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
隐式数对排除法；
*/
void DifficultyEvaluation::UpdateHiddenPair()
{
    for (int k = 0; k < 9; ++k)
    {
        // kth row
        // 第k行的处理
        // record[i]记录i+1作为候选数出现的位置
        vector<vector<pair<int, int>>> record;
        for (int num = 1; num < 10; num++)
        {
            vector<pair<int, int>> appear;
            for (int col = 0; col < 9; col++)
            {
                if (puzzle[k][col] == 0 && candidate[k][col][num] == 1)
                {
                    pair<int, int> tmp(k, col);
                    appear.push_back(tmp);
                }
            }
            record.push_back(appear);
        }
        // 寻找两个出现了两次且出现位置相同的候选数（隐式数对）
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    // 如果在第k行中，候选数i+1，j+1出现2次且位置相同
                    // 删除这两个位置的其余候选数
                    for (int n = 1; n < 10; n++)
                    {
                        if (n != i + 1 && n != j + 1)
                        {
                            CandidateDelete(record[i][0].first, record[i][0].second, n);
                            CandidateDelete(record[i][1].first, record[i][1].second, n);
                        }
                    }
                }
            }
        }
        // kth column
        // 第k列的处理：思路同第k行
        record.clear();
        for (int num = 1; num < 10; num++)
        {
            vector<pair<int, int>> appear;
            for (int row = 0; row < 9; row++)
            {
                if (puzzle[row][k] == 0 && candidate[row][k][num] == 1)
                {
                    pair<int, int> tmp(row, k);
                    appear.push_back(tmp);
                }
            }
            record.push_back(appear);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    for (int n = 1; n < 10; n++)
                    {
                        if (n != i + 1 && n != j + 1)
                        {
                            CandidateDelete(record[i][0].first, record[i][0].second, n);
                            CandidateDelete(record[i][1].first, record[i][1].second, n);
                        }
                    }
                }
            }
        }
        // kth cell
        // 第k宫的处理
        record.clear();
        // rs,cs为第k宫的起始位置
        int rs = 3 * (k / 3);
        int cs = 3 * (k % 3);
        // 下面的操作同上
        for (int num = 1; num < 10; num++)
        {
            vector<pair<int, int>> appear;
            for (int row = rs; row < rs + 3; row++)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                    {
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            record.push_back(appear);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 9; j++)
            {
                if (record[i].size() == 2 && record[j].size() == 2 && record[i][0] == record[j][0] && record[i][1] == record[j][1])
                {
                    for (int n = 1; n < 10; n++)
                    {
                        if (n != i + 1 && n != j + 1)
                        {
                            CandidateDelete(record[i][0].first, record[i][0].second, n);
                            CandidateDelete(record[i][1].first, record[i][1].second, n);
                        }
                    }
                }
            }
        }
    }
}

/*
XWing排除法；
*/
void DifficultyEvaluation::UpdateXWing()
{
    for (int num = 1; num < 10; num++)
    {
        // search by row；按行查找
        for (int row = 0; row < 8; row++)
        {
            // appear记录num在行row出现的位置
            vector<pair<int, int>> appear;
            for (int col = 0; col < 9; col++)
            {
                if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                {
                    pair<int, int> tmp(row, col);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() != 2)
                continue;
            // num出现2次（潜在的xwing），在下面几行寻找num同样出现2次的行
            for (int nrow = row + 1; nrow < 9; nrow++)
            {
                // nAppear记录num在行nrow出现的位置
                vector<pair<int, int>> nAppear;
                for (int col = 0; col < 9; col++)
                {
                    if (puzzle[nrow][col] == 0 && candidate[nrow][col][num] == 1)
                    {
                        pair<int, int> tmp(nrow, col);
                        nAppear.push_back(tmp);
                    }
                }
                if (nAppear.size() != 2)
                    continue;
                // 找到num出现两次的行，则判断四个位置是否构成矩形
                if (appear[0].second == nAppear[0].second&&appear[1].second == nAppear[1].second)
                {
                    // 删除四个边的除了四个角位置的候选数num
                    for (int k = appear[0].second + 1; k < appear[1].second; k++)
                    {
                        CandidateDelete(row, k, num);
                        CandidateDelete(nrow, k, num);
                    }
                    for (int k = row + 1; k < nrow; k++)
                    {
                        CandidateDelete(k, appear[0].second, num);
                        CandidateDelete(k, appear[0].second, num);
                    }
                }
            }
        }
        // search by col： 按列查找（结构类似）
        for (int col = 0; col < 8; col++)
        {
            vector<pair<int, int>> appear;
            for (int row = 0; row < 9; row++)
            {
                if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                {
                    pair<int, int> tmp(row, col);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() != 2)
                continue;
            for (int ncol = col + 1; ncol < 9; ncol++)
            {
                vector<pair<int, int>> nAppear;
                for (int row = 0; row < 9; row++)
                {
                    if (puzzle[row][ncol] == 0 && candidate[row][ncol][num] == 1)
                    {
                        pair<int, int> tmp(row, ncol);
                        nAppear.push_back(tmp);
                    }
                }
                if (nAppear.size() != 2)
                    continue;
                if (appear[0].first == nAppear[0].first&&appear[1].first == nAppear[1].first)
                {
                    for (int k = appear[0].first + 1; k < appear[1].first; k++)
                    {
                        CandidateDelete(k, col, num);
                        CandidateDelete(k, ncol, num);
                    }
                    for (int k = col + 1; k < ncol; k++)
                    {
                        CandidateDelete(appear[0].first, k, num);
                        CandidateDelete(appear[0].first, k, num);
                    }
                }
            }
        }
    }
}

/*
判断数独是否已解出；
返回true表示已解决，返回false表示未解决；
*/
bool DifficultyEvaluation::IsFinished()
{
    bool r = true;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            r &= puzzle[i][j] != 0;
        }
    }
    return r;
}

/*
在puzzle的（row，col）位置填上num；
*/
void DifficultyEvaluation::Fill(int row, int col, int num)
{
    puzzle[row][col] = num;
    candidate[row][col][0] = 0;
}

/*
唯余法填数：寻找只有一个候选数的空格填之；
返回true：填数成功；
返回false：未找到可以填数的空格；
注意：函数只填一个空格即返回；
*/
bool DifficultyEvaluation::FillNakedSingle()
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (puzzle[i][j] == 0 && candidate[i][j][0] == 1)
            {
                // 寻找到一个候选数数量为1的空格，填之
                for (int num = 1; num < 10; num++)
                {
                    if (candidate[i][j][num] == 1)
                    {
                        Fill(i, j, num);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/*
隐式唯余：寻找数独子结构（行列宫）中只出现在一个位置的候选数，则在该位置填该候选数；
返回true：填数成功；
返回false：未找到可以填数的空格；
注意：函数只填一个空格即返回；
*/
bool DifficultyEvaluation::FillHiddenSingle()
{
    for (int num = 1; num < 10; num++)
    {
        // num遍历候选数
        for (int k = 0; k < 9; k++)
        {
            vector<pair<int, int>> appear;
            // kth row
            // appear记录num在k行出现的位置
            for (int col = 0; col < 9; col++)
            {
                if (puzzle[k][col]==0 && candidate[k][col][num] == 1)
                {
                    pair<int, int> tmp(k, col);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() == 1)
            {
                // 只出现1次，则该位置只能填num
                Fill(appear[0].first, appear[0].second, num);
                return true;
            }
            appear.clear();
            // kth column
            // appear记录num在k列出现的位置
            for (int row = 0; row < 9; ++row)
            {
                if (puzzle[row][k]==0 && candidate[row][k][num] == 1)
                {
                    pair<int, int> tmp(row, k);
                    appear.push_back(tmp);
                }
            }
            if (appear.size() == 1)
            {
                // 只出现1次，则该位置只能填num
                Fill(appear[0].first, appear[0].second, num);
                return true;
            }
            appear.clear();
            // kth cell
            // appear记录num在k宫出现的位置
            int rs = 3 * (k / 3);
            int cs = 3 * (k % 3);
            for (int row = rs; row < rs + 3; row++)
            {
                for (int col = cs; col < cs + 3; col++)
                {
                    if (puzzle[row][col] == 0 && candidate[row][col][num] == 1)
                    {
                        pair<int, int> tmp(row, col);
                        appear.push_back(tmp);
                    }
                }
            }
            if (appear.size() == 1)
            {
                // 只出现1次，则该位置只能填num
                Fill(appear[0].first, appear[0].second, num);
                return true;
            }
        }
    }
    return false;
}

/*
使用两种填数策略进行尝试；
其中之一成功返回true，两种策略均失败返回false；
updateCount：计数此次填数对应的排除策略生效次数，填数成功则该策略生效次数加1；
fillCount1：计数NakedSingle填数策略的生效次数，填数成功则该策略生效次数加1；
fillCount2：计数HiddenSingle填数策略生效次数，填数成功则该策略生效次数加1；
*/
bool DifficultyEvaluation::TryToFill(int &updateCount, int &fillCount1, int &fillCount2)
{
    if (FillNakedSingle())
    {
        updateCount++;
        fillCount1++;
        return true;
    }
    else if (FillHiddenSingle())
    {
        updateCount++;
        fillCount2++;
        return true;
    }
    return false;
}

/*
对输入的数独谜题进行难度评估；
*/
Difficulty DifficultyEvaluation::Evaluate(int p[][9])
{
    PuzzleInit(p);
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, f1 = 0, f2 = 0;
    int count = 0;
    while (!IsFinished())
    {
        UpdateSingle();
        if (TryToFill(c1, f1, f2))
            continue;
        UpdateCell();
        if (TryToFill(c2, f1, f2))
            continue;
        UpdateNakedPair();
        if (TryToFill(c3, f1, f2))
            continue;
        UpdateHiddenPair();
        if (TryToFill(c4, f1, f2))
            continue;
        UpdateXWing();
        if (TryToFill(c5, f1, f2))
            continue;
        break;
    }
    cout << c1 << ' ' << c2 << ' ' << c3 << ' ' << c4 << ' ' << c5 << endl;
    cout << f1 << ' ' << f2 << endl;
    double pr = double(f1) / double(f2);
    bool usedUpperStra = c2 != 0 || c3 != 0 || c4 != 0 || c5 != 0;
    bool isHard = usedUpperStra || !IsFinished() || pr < 1.8;
    bool isMedium = !usedUpperStra && IsFinished() && pr >= 1.8 && pr <= 10;
    bool isEasy = !usedUpperStra && IsFinished() && pr>10;
    if (isEasy)
        return Difficulty::EASY;
    else if (isMedium)
        return Difficulty::NORMAL;
    else
        return Difficulty::HARD;
}

void DifficultyEvaluation::GetPuzzle(int p[][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            p[i][j] = puzzle[i][j];
        }
    }
}