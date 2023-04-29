#include "incl.h"

int dynamicMemorySizeCal(int size) {
    return (int)(log10((double)size));
}

char* convertIntToString(int Int) {
    char* ptr = malloc(dynamicMemorySizeCal(Int)); //make sure enough space is available for ptr to store Int
    initializeBuffer(ptr, dynamicMemorySizeCal(Int));
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
    char* messageBuffer = malloc(count+16);
    initializeBuffer(messageBuffer, count+16);
    int index = 0;
    messageBuffer[0] = '$';

    for(; PID[index] != '\0'; ++index)
        messageBuffer[index] = PID[index];
    messageBuffer[index] = '/';
    ++index;

    for(int i = 0; numberOfCharOfInputBuffer[i] != '\0'; ++i) {
        messageBuffer[index] = numberOfCharOfInputBuffer[i];
        ++index;
    }

    messageBuffer[index] = ':';
    ++index;
    
    for(int i = 1; i < count - 1; ++i) {
        messageBuffer[index] = inputBuffer[i];
        ++index;
    }
    free(numberOfCharOfInputBuffer);

    return messageBuffer;
}


int main(int argc, char* argv[])
{
    printf("%ld\n",(long)getpid());
    char* pidString = convertIntToString(getpid());
    char* messageBuffer = createMessage(pidString);
    printf("%s\n", messageBuffer);
    free(pidString);
    free(messageBuffer);
    exit(EXIT_SUCCESS);
}