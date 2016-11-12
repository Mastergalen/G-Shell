#include <stdlib.h>

#define LINE_BUFFER_SIZE 1024

#define ARG_BUFFER_SIZE 64
#define DELIM_SYMBOLS " \n\t\r\a"

char *read_line() {
    int bufferSize = LINE_BUFFER_SIZE;
    int pos = 0;

    char *buffer = malloc(sizeof(char) * bufferSize);

    if(!buffer) {
        perror("Error allocating line buffer");
        exit(-1);
    }

    char character;

    while(1) {
        character = getchar();

        if(character == EOF || character == '\n') {
            buffer[pos] = '\0';
            return buffer;
        }

        buffer[pos] = character;
        pos++;

        //If input exceeds buffer size, we need to expand the buffer size
        if(pos > bufferSize) {
            bufferSize += LINE_BUFFER_SIZE;
            buffer = realloc(buffer, bufferSize * sizeof(char));

            if(!buffer) {
                perror("Error reallocating line buffer");
                exit(-1);
            }
        }
    }
}


char **parse_args(char *line) {
    int bufferSize = ARG_BUFFER_SIZE;

    char **args = malloc(sizeof(char*) * bufferSize);
    char *arg;

    if(!args) {
        perror("Args reallocation error");
        exit(-1);
    }

    arg = strtok(line, DELIM_SYMBOLS);

    int i = 0;
    while(arg != NULL) {
        args[i] = arg;
        i++;

        if(i > bufferSize) {
            bufferSize += ARG_BUFFER_SIZE;

            args = realloc(args, sizeof(char*) * bufferSize);

            if(!args){
                perror("Args reallocation error");
                exit(-1);
            }
        }

        arg = strtok(NULL, DELIM_SYMBOLS);
    }

    args[i] = NULL;

    return args;
}
