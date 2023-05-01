#include "incl.h"

static char delimCheckBuffer[1];

void initializeBuffer(char buffer[], size_t size, size_t index) {
    for(; index < size; ++index) 
        buffer[index] = '\0';
}

long stringToNumber(const char* string) {
    long number = strtol(string, NULL, 10);
    return number;
}

pid_t findPID(const char* string) {
    char buffer[7];
    int indexBuffer = 0;

    initializeBuffer(buffer, 7,0);

    for(int i = 1; string[i] != '/'; ++i) {
        buffer[indexBuffer] = string[i];
        ++indexBuffer;
    }

    return (pid_t) stringToNumber(buffer);
}

size_t findSlash(const char* string, size_t size) {
    size_t indexOfSlash = 0;

    while(indexOfSlash < size && string[indexOfSlash] != '/')
        ++indexOfSlash;
    return indexOfSlash;
}

long findNumOfCharsInMessage(const char* string) {
    long NumOfCharsInMessage;
    char NumOfCharsInMessageString[4];
    size_t dummyVar = 0;

    initializeBuffer(NumOfCharsInMessageString, 4, 0);
    size_t indexOfSlash = findSlash(string, 15)+1;

    while(indexOfSlash < 15 && string[indexOfSlash] != ':') {
        NumOfCharsInMessageString[dummyVar] = string[indexOfSlash];
        ++dummyVar;
        ++indexOfSlash;
    }
    
    NumOfCharsInMessage = stringToNumber(NumOfCharsInMessageString);

    return NumOfCharsInMessage;

}

void readUntilColon(int fd, char* buffer, size_t index, size_t size) {
    while(read(fd, delimCheckBuffer, 1) > 0) {
        if(index < size) {
            if(delimCheckBuffer[0] == ':') {
                buffer[index] = ':';
                break;
            }
            buffer[index] = delimCheckBuffer[0];
            ++index;
            }
        else
            break;
    }
}

char* messageReconstruct(int fdOfFIFO) {
    char* message = calloc(15, sizeof(char));
    long NumOfCharsInMessage;
    size_t index = 0;

    readUntilColon(fdOfFIFO, message, 1, 15);
    NumOfCharsInMessage = findNumOfCharsInMessage(message)+1;
    free(message);
    
    message = calloc(NumOfCharsInMessage, sizeof(char));
    initializeBuffer(message, NumOfCharsInMessage, 0);

    while(read(fdOfFIFO, delimCheckBuffer, 1) > 0) {
    if(index < NumOfCharsInMessage-1) {
        if(delimCheckBuffer[0] == ']')
            break;
        message[index] = delimCheckBuffer[0];
        ++index;
    }
    else 
        break;
    }

    return message;
}

int main(int argc, char* argv[])
{
    char* message;
    int fdOfFIFO;

    fdOfFIFO = open(pathnameOfFIFO, O_RDONLY);

    if(fdOfFIFO == -1)
        errExit("open");

    while(read(fdOfFIFO, delimCheckBuffer, 1) > 0) {
        message = messageReconstruct(fdOfFIFO);

        printf("%s",message);
        free(message);
        message = NULL;
        delimCheckBuffer[0] = '\0';
    }

    exit(EXIT_SUCCESS);

}