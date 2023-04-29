#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "error_functions.c"
#define BUFFSIZE 4096

static char inputBuffer[BUFFSIZE];

void initializeBuffer(char buffer[], int size) {
    for(int i = 0; i < size; ++i)
        buffer[i] = '\0';
}