#pragma once

#include <iostream>
#include <cstdio>

using Rank = int32_t;

const int32_t MAX_N = 10009;
const Rank DEFAULT_CAPACITY = 3;

#define STACK_VECTOR

template <typename T>
struct print
{
    void operator()(T &e) { std::cout << e << " "; }
};