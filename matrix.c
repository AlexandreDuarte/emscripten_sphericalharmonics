#include <math.h>
#include <stdarg.h>
#include <string.h>
#include "matrix.h"

void createMatrix4(Matrix4* matrix) {
    *matrix = (Matrix4) { {1,  0, 0, 0,
                           0,  1, 0, 0,
                           0,  0, 1, 0,
                           0,  0, 0, 1} };
}
void createMatrix3(Matrix3* matrix) {
    *matrix = (Matrix3) { {1,  0, 0,
                           0,  1, 0,
                           0,  0, 1} };
}

void createRotationMatrix4X(Matrix4* matrix, float angle) {
    float x = cosf(angle);
    float y = sinf(angle);
    *matrix = (Matrix4) { {1,  0, 0, 0,
                           0,  x, y, 0,
                           0, -y, x, 0,
                           0,  0, 0, 1} };
}
void createRotationMatrix4Y(Matrix4* matrix, float angle) {
    float x = cosf(angle);
    float y = sinf(angle);
    *matrix = (Matrix4) { {x,  0, -y, 0,
                           0,  1,  0, 0,
                           y,  0,  x, 0,
                           0,  0,  0, 1} };
}
void createRotationMatrix4Z(Matrix4* matrix, float angle) {
    float x = cosf(angle);
    float y = sinf(angle);
    *matrix = (Matrix4) { { x, y, 0, 0,
                           -y, x, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1} };
}  
void createRotationMatrix3X(Matrix3* matrix, float angle) {
    float x = cosf(angle);
    float y = sinf(angle);
    *matrix = (Matrix3) { {1,  0, 0, 
                           0,  x, y, 
                           0, -y, x}};
}
void createRotationMatrix3Y(Matrix3* matrix, float angle) {
    float x = cosf(angle);
    float y = sinf(angle);
    *matrix = (Matrix3) { {x,  0, -y,
                           0,  1,  0,
                           y,  0,  x} };
}
void createRotationMatrix3Z(Matrix3* matrix, float angle) {
    float x = cosf(angle);
    float y = sinf(angle);
    *matrix = (Matrix3) { { x, y, 0,
                           -y, x, 0,
                            0, 0, 1} };
}

void createTranslationMatrix4(Matrix4* matrix, float x, float y, float z) {
    *matrix = (Matrix4) { {1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           x, y, z, 1} };
}

void createScalingMatrix4(Matrix4* matrix, float x, float y, float z) {
    *matrix = (Matrix4) { {x, 0, 0, 0,
                           0, y, 0, 0,
                           0, 0, z, 0,
                           0, 0, 0, 1} };
}

void multMatrices(Matrix4* output, int n_args, ...) {
    
    va_list ap;

    va_start(ap, n_args);

    Matrix4* temp = va_arg(ap, Matrix4*);
    Matrix4* next;

    for(int i = 1; i < n_args; i++) {
        next = va_arg(ap, Matrix4*);

        for(int col = 0; col < 4; col++) {
            for(int row = 0; row < 4; row++) {
                float current = 0.0f;
                for (int offset = 0; offset < 4; offset++) {
                    current += temp->elem[4*offset + row] * next->elem[4*col + offset];
                }
                output->elem[4*col + row] = current;
            }
        }
        memcpy(temp->elem, output->elem, sizeof(float)*16);

    }
    va_end(ap);
}
