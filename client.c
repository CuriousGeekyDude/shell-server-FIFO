#include "incl.h"
#define BUFFSIZE 4096

static char inputBuffer[BUFFSIZE];

int dynamicMemorySizeCal(int size) {
    return (int)(log10((double)size));  //make sure enough space is available for ptr to store Int
}

char* convertIntToString(int Int) {
    char* ptr = calloc(dynamicMemorySizeCal(Int),sizeof(char)); 
    sprintf(ptr, "%d", Int);
    return ptr;
}

int countInput() {
    int countNum;
    read(STDIN_FILENO, inputBuffer, BUFFSIZE-1);

    for(int i = 0; inputBuffer[i] != '\0' && i < BUFFSIZE; ++i) {
        ++countNum;
    }
    return --countNum; //read() reads newline character as well
}

char* createMessage(const char* PID) {
    int count = countInput();
    char* numberOfCharOfInputBuffer = convertIntToString(count); 
    char* messageBuffer = calloc(count+16,sizeof(char)); 
    
    
    messageBuffer[0] = '['; //Used to identify the beginning of the new message

    int index = 1;
    for(int i = 0; PID[i] != '\0'; ++i) {
        messageBuffer[index] = PID[i];      //PID part of the message is constructed
        ++index;
    }
    
    messageBuffer[index] = '/';
    ++index;

    for(int i = 0; numberOfCharOfInputBuffer[i] != '\0'; ++i) {
        messageBuffer[index] = numberOfCharOfInputBuffer[i];      //Number of characters of the input from the user
        ++index;                                                  //is inserted into the message. 
    }

    messageBuffer[index] = ':';
    ++index;

    for(int i = 0; i < count; ++i) {
        messageBuffer[index] = inputBuffer[i];    //The actual input from the user is finally inserted
        ++index;                                     
    }
    free(numberOfCharOfInputBuffer);

    messageBuffer[index] = ']';

    return messageBuffer;
}


int main(int argc, char* argv[])
{
    int fdOfFIFO;
    char* message;

    fdOfFIFO = open(pathnameOfFIFO , O_WRONLY);
    if(fdOfFIFO == -1)
        errExit("open");

    while((message = createMessage(convertIntToString(getpid()))) != NULL) {
        write(fdOfFIFO, message, BUFFSIZE-1);
        printf("%s\n", message);
        free(message);
    }
    
    exit(EXIT_SUCCESS);
}