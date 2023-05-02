#include "incl.h"

static char delimCheckBuffer[1];

long stringToNumber(const char* string) {   //PASSED
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

size_t findFirstSlash(const char* string,const size_t size) {    //PASSED
    size_t indexOfSlash = 0;

    while(indexOfSlash < size && string[indexOfSlash] != '/')
        ++indexOfSlash;
    return indexOfSlash;
}

long findNumOfCharsInMessage(const char* string, const size_t sizeString) { //PASSED
    long NumOfCharsInMessage;
    char NumOfCharsInMessageString[4];
    size_t dummyVar = 0;

    initializeBuffer(NumOfCharsInMessageString, 4, 0);
    size_t indexOfSlash = findFirstSlash(string, sizeString)+1;

    while(indexOfSlash < sizeString && string[indexOfSlash] != ':') {
        NumOfCharsInMessageString[dummyVar] = string[indexOfSlash];
        ++dummyVar;
        ++indexOfSlash;
    }
    
    NumOfCharsInMessage = stringToNumber(NumOfCharsInMessageString);

    return NumOfCharsInMessage;

}

void readUntilColon(int fd, char* buffer, size_t index, size_t size) {  //PASSED
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

    readUntilColon(fdOfFIFO, message, index, 15);
    NumOfCharsInMessage = findNumOfCharsInMessage(message, 15);
    free(message);
    
    message = calloc(NumOfCharsInMessage, sizeof(char));
    initializeBuffer(message, NumOfCharsInMessage, 0);

    while(read(fdOfFIFO, delimCheckBuffer, 1) > 0) {
    if(index < NumOfCharsInMessage) {
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

    while(1) {
        message = messageReconstruct(fdOfFIFO);

        printf("%s\n",message);
        fflush(stdout);
        free(message);
        message = NULL;
        delimCheckBuffer[0] = '\0';
    }

    exit(EXIT_SUCCESS);

}