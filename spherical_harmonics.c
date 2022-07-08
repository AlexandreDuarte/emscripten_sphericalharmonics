#include "spherical_harmonics.h"
#include <math.h>
#include "math_utils.h"
#include <stdlib.h>
#include <stdio.h>

// Legendre Polinominals recursive computation
float computeLegendrePolinominal(float x, float y, int l, int m)
{
    if (l == m) {
        return (1 - 2 * l % 2) * dfactorial(2 * l - 1) * powf(y, l);
    } 
    if (l == m+1) {
        return x * (2 * l + 1) * computeLegendrePolinominal(x, y, l, l);
    }

    return ((2.0f * l - 1.0f) * x * computeLegendrePolinominal(x, y, l - 1, m) - (l - 1 + m)*computeLegendrePolinominal(x, y, l - 2, m))/(l-m);



/*
    *result = 0;

    for (int k = m; k <= l; k++)
    {
        *result += powf(x, 1.0f*(k - m)) * ((1.0f*factorial(k) / 1.0f*factorial(k - m)) * binomialCombination(1.0f*l, k) * binomialCombination((l + k - 1)*0.5f, l));
    }

    *result *= T * powf(y*y, 0.5f*m);*/
}

void computeLegendrePolinominal2(float* result, float x, float y, int l, int m, float T) {
    *result = 0;

    for (int k = l; k >= m; k--)
    {
        *result += binomialCombination(1.0 * l, k) * binomialCombination((l + k - 1)*0.5, l) * 1.0*factorial(k) * powf(x, 1.0*(k - m)) / 1.0*factorial(k - m);
    }

    *result *= T * powf(1 - x*x, m*0.5l);
}

void computeSphericalHarmonic(ObjectData *data, int l, int m)
{

    int SIZE = 402;
    int SIZE2 = 201;

    float T = powf(2.0l, (float)l);

    data->vertex_size = 4 * (SIZE+2) * (SIZE2+3);
    data->indices_size = (SIZE+2) * (SIZE2 + 3) * 2;

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

    float step = (1.0f / SIZE2);

    float x, y, p_x, L;

    float max_L = 0.0f;

    int count = 0;

    // CALCULATE harmonics in spherical coordinates
    for (int phi = 0; phi <= SIZE + 1; phi++)
    {
        p_x = cosf(m*phi*step*M_PI);
        for (int theta = 0; theta <= SIZE2; theta++)
        {
            x = cosf(theta*step*M_PI);
            y = sinf(theta*step*M_PI);

            //computeLegendrePolinominal2(&L, x, y, l, abs(m), T);
            L = computeLegendrePolinominal(x, y, l, abs(m));
            L *= T;

            if (L < 0) {
                if (-L > max_L)
                {
                    max_L = -L;
                }
            } else {
                if (L > max_L)
                {
                    max_L = L;
                }
            }
            

            vd[4 * count] = (float) (y * cosf(phi*step*M_PI));
            vd[4 * count + 1] = (float) (y * sinf(phi*step*M_PI));
            vd[4 * count + 2] = (float) (x);
            vd[4 * count + 3] = L*p_x;

            count++;
        }
        vd[4 * (count)] = vd[4 * (count - 1)];
        vd[4 * (count) + 1] = vd[4 * (count - 1) + 1];
        vd[4 * (count) + 2] = vd[4 * (count - 1) + 2];
        vd[4 * (count) + 3] = vd[4 * (count - 1) + 3];

        vd[4 * (count + 1)] = vd[4 * (count - 1)];
        vd[4 * (count + 1) + 1] = vd[4 * (count - 1) + 1];
        vd[4 * (count + 1) + 2] = vd[4 * (count - 1) + 2];
        vd[4 * (count + 1) + 3] = vd[4 * (count - 1) + 3];

        count+=2;
    }

    // NORMALIZE

    for (int i = 0; i < data->vertex_size / 4; i++)
    {
        vd[4 * i + 0] /= max_L;
        vd[4 * i + 1] /= max_L;
        vd[4 * i + 2] /= max_L;
    }

    count = 0;

    // BUILD POINTS SEQUENCE
        for (unsigned int k = 0; k < SIZE+1; k++)
        {
            for (unsigned int i = 0; i <= SIZE2 + 3; i++)
            {

                id[count] = i + (k) * (SIZE2 + 3);
                id[count + 1] = i + (k + 1) * (SIZE2 + 3);

                count += 2;
            }
        }

}
