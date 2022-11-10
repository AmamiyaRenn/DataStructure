#pragma once

#include "DSA/Macro.h"
#include "DSA/Vector.h"
#include <algorithm>
#include <stdint.h>
#include <type_traits>

// 字符串暴力匹配算法
template<typename T>
Rank match_BruteForce(const Vector<T>& Pattern, const Vector<T>& Text)
{
    Rank n = Text.getSize(), t = 0;    // 文本串指针
    Rank m = Pattern.getSize(), p = 0; // 模式串指针
    while (t < n && p < m)
        if (Text[t] == Pattern[p])
            t++, p++; // 匹配成功，指针后移
        else
            t -= p - 1, p = 0; // 匹配失败，指针倒带
    return t - p;
}

// 构造KMP next表
template<typename T>
static int* buildNext(const Vector<T>& Pattern)
{
    size_t m    = Pattern.getSize();
    Rank   t    = 0;          // “主”串指针
    int*   next = new int[m]; // next表
    Rank   p = next[0] = -1;  // 模式串指针
    while (t < m - 1)
        if (p < 0 || Pattern[t] == Pattern[p])
        {
            t++, p++; // 匹配成功，指针后移
            next[t] =
                (Pattern[t] == Pattern[p] ? next[t - 1] : t - 1); // 确定下一个位置的next（吸取经验和教训的迭代方法）
        }
        else
            p = next[p]; // 根据next表的定义，移动指针
    return next;
}

// 字符串KMP匹配算法
template<typename T>
Rank match_KMP(const Vector<T>& Pattern, const Vector<T>& Text)
{
    int* next = buildNext(Pattern);    // next表
    Rank n = Text.getSize(), t = 0;    // 文本串指针
    Rank m = Pattern.getSize(), p = 0; // 模式串指针
    while (t < n && p < m)
        if (p < 0 || Text[t] == Pattern[p])
            t++, p++; // 匹配成功，指针后移
        else
            p = next[p]; // 根据next表的定义，移动指针
    delete[] next;
    return t - p;
}

// 构造BM bad character表
template<typename T>
static int* buildBC(const Vector<T>& Pattern)
{
    uint32_t n  = sizeof(T) * 8; // 字符集个数
    uint32_t m  = Pattern.getSize();
    Rank     p  = 0;          // 模式串指针
    int*     bc = new int[n]; // bad character table
    for (uint32_t i = 0; i < n; i++)
        bc[i] = -1;
    for (uint32_t i = 0; i < m; i++) // 画家算法更新bc项
        bc[Pattern[i]] = i;
    return bc;
}

// 构造BM suffix size(最大匹配后缀长度)表
template<typename T>
static int* buildSS(const Vector<T>& Pattern)
{
    size_t m  = Pattern.getSize();
    int*   ss = new int[m]; // suffix size table
    ss[m - 1] = m;          // 对于最后一个字符而言，其最大匹配后缀长度就是整个Pattern的长度
    for (int low = m - 1, high = m - 1, i = low - 1; 0 <= i; i--)
        if (low < i && ss[m - high + i - 1] < i - low) // 此时，ss[i]最大也就和ss[m-1-(high-i)]相同
            ss[i] = ss[m - high + i - 1];
        else // 此时，ss[i]=最大匹配后缀长度
        {
            high = i;
            low  = std::min(low, high);
            while (0 <= low && Pattern[low] == Pattern[m - high + low - 1])
                low--;
            ss[i] = high - low;
        }
    return ss;
}

// 构造BM good suffix(好后缀)表
template<typename T>
static int* buildGS(const Vector<T>& Pattern)
{
    int*   ss = buildSS(Pattern);
    size_t m  = Pattern.getSize();
    int*   gs = new int[m]; // Good suffix shift table
    for (size_t i = 0; i < m; i++)
        gs[i] = m; // 初始化
    for (size_t i = 0, j = m - 1; j < UINTMAX_MAX; j--)
        if (ss[j] == j + 1)            // MS[j]就是整个前缀Pattern[0, j]
            while (i < m - j - 1)      // Pattern[m-j-1]左侧的每个gs
                gs[i++] = m - j - 1;   // 都可以直接位移m-j-1个单位从而与前缀对齐
    for (size_t j = 0; j < m - 1; j++) // 画家算法：gs保守化
        gs[m - ss[j] - 1] = m - j - 1; // 同样是位移m-j-1个单位
    delete[] ss;
    return gs;
}

// 字符串BM匹配算法
template<typename T>
Rank match_BM(const Vector<T>& Pattern, const Vector<T>& Text)
{
    int*   bc = buildBC(Pattern);
    int*   gs = buildGS(Pattern);
    size_t n = Text.getSize(), m = Pattern.getSize();
    Rank   t = 0; // 文本串指针
    while (t + m <= n)
    {
        Rank p = m - 1;                   // 模式串指针，从模式串末尾开始匹配
        while (Text[t + p] == Pattern[p]) // 自右向左比对
            if (--p < 0)
                break;
        if (p < 0) // 如果p<0，说明发生了完全匹配
            break;
        else // 否则，需要移动模式串，移动量由bc与gs表决定
            t += std::max(gs[p], p - bc[Text[t + p]]);
    }
    delete[] bc;
    delete[] gs;
    return t;
}