#include "spherical_harmonics.h"
#include <math.h>
#include "math_utils.h"
#include <stdlib.h>
#include <stdio.h>

// Legendre Polinominals recursive computation
double computeLegendrePolinominal(double x, double y, int l, int m)
{
    if (l == m) {
        return (1-2*l%2) * dfactorial(2*l - 1) * pow(y*y, m*0.5);
    } 
    if (l == m+1) {
        return x*(2*l+1)*computeLegendrePolinominal(x, y, l, l);
    }

    return ((2.0*l -1.0)*x*computeLegendrePolinominal(x, y, l-1, m) - (l-1+m)*computeLegendrePolinominal(x, y, l-2, m))/(l-m);



/*
    *result = 0;

    for (int k = m; k <= l; k++)
    {
        *result += powf(x, 1.0f*(k - m)) * ((1.0f*factorial(k) / 1.0f*factorial(k - m)) * binomialCombination(1.0f*l, k) * binomialCombination((l + k - 1)*0.5f, l));
    }

    *result *= T * powf(y*y, 0.5f*m);*/
}

void computeLegendrePolinominal2(double* result, double x, double y, int l, int m, double T) {
    *result = 0;

    for (int k = l; k >= m; k--)
    {
        *result += binomialCombination(1.0*l, k) * binomialCombination((l + k - 1)*0.5, l) * 1.0*factorial(k) * pow(x, 1.0*(k - m)) / 1.0*factorial(k - m);
    }

    *result *= T * pow(1 - x*x, m*0.5);
}

void computeSphericalHarmonic(ObjectData *data, int l, int m)
{

    int SIZE = 402;
    int SIZE2 = 201;

    double T = pow(2.0, (double)l);

    data->vertex_size = 2 * 4 * (SIZE+1) * (SIZE2+1);
    data->indices_size = (SIZE) * (SIZE2) * 2;

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

    double step = (1.0 / SIZE2);

    double x, y, p_x, L;

    double max_L = 0.0;

    int count = 0;

    // CALCULATE harmonics in spherical coordinates
    for (int phi = 0; phi <= SIZE; phi++)
    {
        p_x = 1.0;
        for (int theta = 0; theta <= SIZE2; theta++)
        {
            x = cos(theta*step*M_PI);
            y = sin(theta*step*M_PI);

            computeLegendrePolinominal2(&L, x, y, l, abs(m), T);


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

            double amplitude = (1 - 2*((L * p_x)<0.0)) * (L * p_x);
            

            vd[4 * count] = (float) (amplitude* y * cos(((double)phi*step)*M_PI));
            vd[4 * count + 1] = (float) (amplitude* y * sin(((double)phi*step)*M_PI));
            vd[4 * count + 2] = (float) (amplitude * x);
            vd[4 * count + 3] = (L*p_x*x<0.0)*1.0f;

            count++;
        }
    }

    // NORMALIZE
    for (int i = 0; i < data->vertex_size/4; i++)
    {
        vd[4*i + 0] /= max_L;
        vd[4*i + 1] /= max_L;
        vd[4*i + 2] /= max_L;
    }

    count = 0;

    // BUILD POINTS SEQUENCE
        for (unsigned int k = 0; k < SIZE; k++)
        {
            for (unsigned int i = 0; i < SIZE2; i++)
            {

                id[count] = i + (k) * (SIZE2);
                id[count + 1] = i + (k + 1) * (SIZE2);

                count += 2;
            }
        }

}
