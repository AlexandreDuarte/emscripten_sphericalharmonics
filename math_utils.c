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

float factorialk(float alpha, int k)
{
    float result = 1.0f;
    for (int i = 0; i < k; i++)
    {
        result *= (alpha - (float)i);
    }
    return result;
}

float binomialCombination(float n, int k)
{
    return factorialk(n, k) / (float)factorial(k);
}