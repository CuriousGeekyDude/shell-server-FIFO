#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "error_functions.c"
#define pathnameOfFIFO "/home/feri/Desktop/myfifo"

void initializeBuffer(char buffer[], size_t size, size_t index) {
    for(; index < size; ++index) 
        buffer[index] = '\0';
}


