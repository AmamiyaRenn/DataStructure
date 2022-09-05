#include <iostream>
#include <ctime>
#include "inc/Complex.h"

using namespace std;

long long Sum1(long long n)
{
    long long s = 0;
    for (long long i = 1; i <= n; i++)
        s += i;
    return s;
}

long long Sum2(long long n)
{
    return n * (n + 1) / 2;
}

long long calFactorialSumUsingSum1(long long n)
{
    long long ans = 0;
    for (long long i = 1; i <= n; i++)
        ans += Sum1(i);
    return ans;
}

long long calFactorialSumUsingSum2(long long n)
{
    long long ans = 0;
    for (long long i = 1; i <= n; i++)
        ans += Sum2(i);
    return ans;
}

void question1(long long n)
{
    clock_t tCost = clock();
    cout << calFactorialSumUsingSum1(n) << " ";
    cout << (double)(clock() - tCost) / CLOCKS_PER_SEC << endl;

    tCost = clock();
    cout << calFactorialSumUsingSum2(n) << " ";
    cout << (double)(clock() - tCost) / CLOCKS_PER_SEC << endl;
}

long long solution1(long long n)
{
    long long ans = 0;
    for (long long i = 1; i <= n; i++)
    {
        long long temp = 1;
        for (long long j = 2; j <= i; j++)
            temp *= j;
        ans += temp;
    }
    return ans;
}

long long solution2(long long n)
{
    long long ans = 1, temp = 1;
    for (long long i = 2; i <= n; i++)
    {
        temp *= i;
        ans += temp;
    }
    return ans;
}

void FirstAndSecondMaximum(int a[], int n)
{
    for (int i = 0; i < 2; i++)
        for (int j = 1; j < n; j++)
            if (a[j - 1] > a[j])
                swap(a[j - 1], a[j]);
    cout << "First and second maximum is " << a[n - 1] << " " << a[n - 2] << endl;
}

int main()
{
    question1(100000);
    // question1(3);
    // cout << solution1(4) << endl; // O(n^2)复杂度
    // cout << solution2(4) << endl; // O(n)复杂度
    // int a[] = {1, 5, 10, 4, 9, 6, 2};
    // FirstAndSecondMaximum(a, 7);
    Complex c1(1, 2), c2(3, 4);
    cout << "Complex c1: " << c1 << endl;
    cout << "Complex c2: " << c2 << endl;
    cout << "Complex c1 + c2: " << c1 + c2 << endl;
    cout << "Complex c1 - c2: " << c1 - c2 << endl;
    cout << "Complex c1 * c2: " << c1 * c2 << endl;

    return 0;
}