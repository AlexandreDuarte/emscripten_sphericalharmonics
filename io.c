#include <stdio.h>
#include <stdlib.h>

char* loadFile(char* path) {

    FILE* fs = fopen(path, "r");
    
    if (!fs) {
        printf("%s file not found!", path);
    }

    fseek(fs, 0, SEEK_END);
    long fs_size = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    
    char* buffer = malloc(fs_size+1);
    fread(buffer, fs_size, 1, fs);
    fclose(fs);
    buffer[fs_size] = 0;

    return buffer;
}