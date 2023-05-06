#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "error_functions.c"
#define pathnameOfFIFO "/home/feri/Desktop/myfifo"

void initializeBuffer(char buffer[], const size_t size, size_t index) {
    for(; index < size; ++index) 
        buffer[index] = '\0';
}

int dynamicMemorySizeCal(int size) {
    return (int)(((ceil(log10((double)size)))*sizeof(char)) + 6);  //make sure enough space is available for ptr to store Int
}

char* convertIntToString(const int Int) {
    int sizeNeeded = dynamicMemorySizeCal(Int);
    char* ptr = calloc(sizeNeeded,sizeof(char)); 
    initializeBuffer(ptr, sizeNeeded, 0);
    sprintf(ptr, "%d", Int);
    return ptr;
}

char* createFIFOPathNameClient(const pid_t PIDClient) {
    char* PIDClientString = convertIntToString((int)PIDClient);
    char* pathName = malloc(35);
    initializeBuffer(pathName, 35, 0);

    char buffDummy[27] = "/home/feri/Desktop/logFIFO/";
    for(int i = 0; i < 27; ++i) 
        pathName[i] = buffDummy[i];
    
    for(int i = 27; i < 34; ++i)
        pathName[i] = PIDClientString[i-27];
    
    free(PIDClientString);
    return pathName;
}

