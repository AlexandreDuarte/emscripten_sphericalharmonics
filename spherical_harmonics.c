#include "spherical_harmonics.h"
#include <math.h>
#include "math_utils.h"
#include <stdlib.h>
#include <stdio.h>

// Legendre Polinominals iterative aproximation
void computeLegendrePolinominal(float *result, float x, float y, float l, float m, float T)
{

    *result = 0;

    for (float k = m; k <= l; k++)
    {
        *result += powf(x, k - m) * (float)(factorial(k) * binomialCombination(l, k) * binomialCombination((float)(l + k - 1) / 2, l) / factorial(k - m));
    }

    *result *= T * powf(y, m);
}

void computeSphericalHarmonic(ObjectData *data, int l, int m)
{

    int SIZE = 402;
    int SIZE2 = 201;

    float T = powf((float)2, (float)l);

    data->vertex_size = 6 * SIZE * SIZE;
    data->indices_size = SIZE * SIZE * 4;

    float *vd = malloc(sizeof(float) * data->vertex_size);
    if (!vd)
    {
        printf("Failed vertex memory allocation\n");
    }
    data->vertex_data = vd;
    unsigned int *id = malloc(sizeof(unsigned int) * data->indices_size);
    if (!id)
    {
        printf("Failed indices memory allocation\n");
    }
    data->indices_data = id;

    float mm = m * (1 - 2 * (m < 0));

    float step = (1.0f / (float)(SIZE2));

    float x, y, p_x, L;

    float max_L = 0.0f;

    int count = 0;

    // CALCULATE harmonics in spherical coordinates
    for (int phi = 0; phi <= SIZE; phi++)
    {
        p_x = cosf(mm*phi*step*M_PI);
        for (int theta = 0; theta <= SIZE2; theta ++)
        {
            x = cosf(theta*step*M_PI);
            y = sinf(theta*step*M_PI);

            computeLegendrePolinominal(&L, x, y, (float)l, mm, T);

            if (L > max_L)
            {
                max_L = L;
            }

            vd[3 * count] = L * p_x * y * sinf(phi*step*M_PI);
            vd[3 * count + 1] = L * p_x * y * cosf(phi*step*M_PI);
            vd[3 * count + 2] = L * p_x * x;

            count++;
        }
    }

    count = 0;

    for (int phi = 0; phi <= SIZE; phi++)
    {
        p_x = cosf(mm*phi*step*M_PI);
        for (int theta = 0; theta <= SIZE2; theta++)
        {
            x = cosf(theta*step*M_PI);
            y = sinf(theta*step*M_PI);

            computeLegendrePolinominal(&L, x, y, (float)l, mm, T);

            if (L > max_L)
            {
                max_L = L;
            }

            vd[3 * count] = -L * p_x * y * sinf(phi*step*M_PI);
            vd[3 * count + 1] = -L * p_x * y * cosf(phi*step*M_PI);
            vd[3 * count + 2] = -L * p_x * x;

            count++;
        }
    }

    // NORMALIZE
    for (int i = 0; i < data->vertex_size; i++)
    {
        vd[i] /= max_L;
    }

    count = 0;
    unsigned int offset = 0;

    // BUILD POINTS SEQUENCE
    for (int loop = 0; loop <= 1; loop++)
    {
        offset = count / 2;
        for (unsigned int k = 0; k < SIZE; k++)
        {
            for (unsigned int i = 0; i < SIZE2; i++)
            {

                id[count] = offset + i + (k) * (SIZE2);
                id[count + 1] = offset + i + (k + 1) * (SIZE2);

                count += 2;
            }
        }
    }
}
