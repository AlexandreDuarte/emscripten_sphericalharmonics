#include "math_utils.h"

int factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

int dfactorial(int n) {
    int result = 1;
    for (int i = n; i > 0; i-=2) {
        result *= i;
    }
    return result;
}

double factorialk(double alpha, int k)
{
    double result = 1.0f;
    for (int i = 0; i < k; i++)
    {
        result *= (alpha - (double)i);
    }
    return result;
}

double binomialCombination(double n, int k)
{
    return factorialk(n, k) / (double)factorial(k);
}