typedef struct
{
    float elem[16];
} Matrix4;
typedef struct
{
    float elem[9];
} Matrix3;


void createMatrix4(Matrix4*);
void createMatrix3(Matrix3*);

void createRotationMatrix4X(Matrix4*, float);
void createRotationMatrix4Y(Matrix4*, float);
void createRotationMatrix4Z(Matrix4*, float);

void createRotationMatrix3X(Matrix3*, float);
void createRotationMatrix3Y(Matrix3*, float);
void createRotationMatrix3Z(Matrix3*, float);

void createTranslationMatrix4(Matrix4*, float, float, float);
void multMatrices(Matrix4*, int, ...);