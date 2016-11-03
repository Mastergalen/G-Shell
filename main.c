#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

#define LINE_BUFFER_SIZE 1024

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
            buffer = realloc(buffer, bufferSize);

            if(!buffer) {
                perror("Error reallocating line buffer");
                exit(-1);
            }
        }
    }
}

void command_loop(char cwd[]) {
    char *line;

    while(1) {
        printf("%s > ", cwd);

        line = read_line();

        if(strcmp("exit", line) == 0) {
            break;
        }
    }
}

void print_welcome() {
    struct passwd *p = getpwuid(getuid());  // Check for NULL!
    printf("Welcome: %s!\n", p->pw_name);
}

void load_profile() {
    //TODO Throw error if there is no profile file
}

int main(int argc, char **argv) {
    print_welcome();

    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        exit(-1);
    }

    command_loop(cwd);

    printf("Thanks, come again\n");

    return 0;
}
