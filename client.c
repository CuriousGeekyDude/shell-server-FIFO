#include "incl.h"
#define BUFFSIZE 4096


static char inputBuffer[BUFFSIZE];
static int messageSize;

void signalHandler(int sig) {
    pid_t PID = getpid();
    int fdFIFO;

    printf("signal received\n");
    fflush(stdout);
    char* pathName = createFIFOPathNameClient(PID);
    fdFIFO = open(pathName, O_RDONLY);
    initializeBuffer(inputBuffer, BUFFSIZE, 0);
    read(fdFIFO, inputBuffer, BUFFSIZE-1);
    write(STDOUT_FILENO, inputBuffer, BUFFSIZE-1);
    
}

int countInput() {
    int countNum;
    read(STDIN_FILENO, inputBuffer, BUFFSIZE-1);

    for(int i = 0; inputBuffer[i] != '\0' && i < BUFFSIZE; ++i) {
        ++countNum;
    }
    return --countNum; //read() reads newline character as well
}

void writePIDIntoMessage(const char* PID, char* message, size_t sizeMessage, size_t* index) {
    message[*index] = '[';
    ++(*index);
    for(int i = 0; PID[i] != '\0'; ++i) {
        if((*index) < sizeMessage) {
        message[*index] = PID[i];      
        ++(*index);
        }
        else 
            break;
    }    
}

void writeNumOfCharsIntoMessage(const char* NumOfCharsOfInput, char* message, size_t messageSize, size_t* index) {

    message[*index] = '/';
    ++(*index);

    for(int i = 0; NumOfCharsOfInput[i] != '\0'; ++i) {
        if((*index) < messageSize) {
        message[*index] = NumOfCharsOfInput[i];      
        ++(*index);
        } 
        else 
            break;                                       
    }

    if((*index) < messageSize) {
    message[*index] = ':';
    ++(*index);
    }
}

void writeMessageIntoMessageBuff(const char* input, char* message, size_t messageSize, size_t* index) {
    for(int i = 0; i < messageSize-16; ++i) {
        if((*index) < messageSize) {
        message[*index] = inputBuffer[i];   
        ++(*index);
        }
        else
            break;                                     
    }
    message[*index] = ']';

}

char* createMessage(const char* PID) {
    int count = countInput() + 16;
    messageSize = count-16;
    char* numberOfCharOfInputBuffer = convertIntToString(messageSize); 
    char* messageBuffer = calloc(count,sizeof(char)); 
    size_t index = 0;

    initializeBuffer(messageBuffer, count, 0);
    writePIDIntoMessage(PID, messageBuffer, count, &index);
    writeNumOfCharsIntoMessage(numberOfCharOfInputBuffer, messageBuffer, count, &index);
    writeMessageIntoMessageBuff(inputBuffer, messageBuffer, count, &index);

    free(numberOfCharOfInputBuffer);
    return messageBuffer;
}


int main(int argc, char* argv[])
{   
    struct sigaction sigAct;
    int fdOfFIFO;
    char* message;

    memset(&sigAct, 0, sizeof(struct sigaction));
    sigAct.sa_handler = &signalHandler;
    if(sigaction(SIGINT, &sigAct, NULL) == -1)
        errExit("sigaction");

    initializeBuffer(inputBuffer, BUFFSIZE, 0);

    fdOfFIFO = open(pathnameOfFIFO , O_WRONLY);
    if(fdOfFIFO == -1)
        errExit("open");

    while(1) {
        message = createMessage(convertIntToString(getpid()));
        write(fdOfFIFO, message, messageSize+16);
        printf("%s\n", message);
        fflush(stdout);
        free(message);
        initializeBuffer(inputBuffer, BUFFSIZE, 0);
        messageSize = 0;
        message = NULL;
    }
    
    exit(EXIT_SUCCESS);
}