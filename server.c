#include "incl.h"

static char delimCheckBuffer[1];
static pid_t PID;

long stringToNumber(const char* string) {   //PASSED
    long number = strtol(string, NULL, 10);
    return number;
}

pid_t findPID(const char* string, const size_t sizeString) {    //PASSED
    
    char buffer[7];
    int indexBuffer = 0;

    initializeBuffer(buffer, 7, 0);
    
    if(string[0] != '\0') {
    for(int i = 1; i < sizeString && string[i] != '/'; ++i) {
        if(indexBuffer < 7) {
            buffer[indexBuffer] = string[i];
            ++indexBuffer;
        }
        else
            break;
    }
    return (pid_t) stringToNumber(buffer);
    }
    else
        return PID;
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

void readUntilColon(const int fd, char* buffer, size_t index, const size_t size) {  //PASSED
    int round = 1;
    while(read(fd, delimCheckBuffer, 1) > 0) {

/*The message sent by the client may have null characters at the end.
This function only traverses the sent message until its colon. The 
actual message is read into a buffer by the messageReconstruct function. 
So some of the null characters at the end of the sent message are not 
processed, depending on the number of digits of the PID and length of 
the actual message. Therefore we need to make sure when we read the next 
message, we eliminates these null characters first until we actually 
reach the '[' character.*/

        if(round == 1) {
            if(delimCheckBuffer[0] == '[') {
                ++round;
                goto label;
            }
            else 
                continue;
        }

        label:
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

char* messageReconstruct(int fdOfFIFO) {    //PASSED
    char* message = calloc(16, sizeof(char));
    long NumOfCharsInMessage;
    size_t index = 0;
    initializeBuffer(message, 16, 0);

    readUntilColon(fdOfFIFO, message, index, 16);
    PID = findPID(message, 16);
    NumOfCharsInMessage = findNumOfCharsInMessage(message, 16);
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

/*In order to prevent the server from printing newline character
indefinitely in case all of the clients disconnect from the other 
end of the FIFO.*/

        if(message[0] != '\0') {
            printf("%d:%s\n",PID,message);
            fflush(stdout);
        }

        if(message != NULL)
            free(message);

        message = NULL;
        delimCheckBuffer[0] = '\0';
    }
    

    exit(EXIT_SUCCESS);

}