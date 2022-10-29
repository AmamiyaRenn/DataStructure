#pragma once

#include "Macro.h"

class Fibonacci
{
public:
    explicit Fibonacci(Rank n) : f(1), g(0)
    {
        while (g < n)
            next();
    }
    // 获得fib(k)
    Rank get() const { return g; }
    // 转到下一项fib(k+1)，并返回fib(k+1)
    Rank next()
    {
        g += f, f = g - f;
        return g;
    }
    // 转到上一项fib(k-1)，并返回fib(k-1)
    Rank prev()
    {
        f = g - f, g -= f;
        return g;
    }

private:
    Rank f, g; // f=fib(k-1), g=fib(k);
};
