# Shell-server-FIFO

## About
This is a simple server client program. The server is run first and then the client follows subsequently. The server listens for any messages that are sent from the client through a FIFO. After the client sends a message, the server process the command using popen() and then signals the client and then creates a file in logFifo directory in the home directory with the PID of the client as its name. The signal is used to notify the client to access the created file in order to read the result of the command and display it in the terminal.

## OS
Linux

## Language
C99

## How to use
Make sure that you have sudo privilage when executing the program. Build both client and server. Then first execute server and then execute as many clients as you see fit.
