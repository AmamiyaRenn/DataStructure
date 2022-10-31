#pragma once

#include <string.h>

///\brief 筛法计算返回[low, max)中最小的素数
int prime(int low, int max)
{
    bool is_not_prime[max];
    memset(is_not_prime, false, max);
    for (int i = 2; i < max; i++)
    {
        if (!is_not_prime[i])
            for (int prime = 2 * i; prime < max; prime += i)
                is_not_prime[prime] = true;
    }
    for (int i = low; i < max; i++)
        if (!is_not_prime[i])
            return i;
    return 0; // 计算失败
}
