#pragma once

#include <iostream>
#include <cstdio>
#include <limits.h>

using Rank = int32_t;

const int32_t MAX_N = 10009;
const Rank DEFAULT_CAPACITY = 2;

#define STACK_VECTOR

template <typename T>
struct Print
{
    void operator()(T &e) { std::cout << e << " "; }
};