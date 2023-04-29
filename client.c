#include "incl.h"

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
    char* numberOfCharOfInputBuffer = convertIntToString(count-2); //not taing into account the $ delimiters
    char* messageBuffer = calloc(count+18,sizeof(char)); //should have been +16 but because of -2 of above, we had to account for the lost 2
    
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
    
    char* message = createMessage(pidString);

    printf("%s\n", message);
    free(pidString);
    free(message);
    exit(EXIT_SUCCESS);
}