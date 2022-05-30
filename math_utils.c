#include "math_utils.h"

float factorial(float n)
{
    float result = 1;
    for (float i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

float factorialk(float n, float k)
{
    float result = 1;
    for (float i = n - k + 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

float binomialCombination(float n, float k)
{
    return factorialk(n, k) / factorial(k);
}