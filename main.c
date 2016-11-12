#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>

#include "profileParser.h"

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

/**
 * Executes a command by spawning a new process
 * @param  command Command to execute
 * @return      Return status
 */
int execute_command(char **args) {
    pid_t pid, waitingPid;
    int status;

    pid = fork();

    if(pid < 0) {
        perror("Error forking process");
    } else if (pid == 0) {
        //child process, execute the command

        if(execvp(args[0], args) == -1) {
            perror("Error executing command");
        }


    } else {
        //Parent process
        do {
            waitingPid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void command_loop(char cwd[]) {
    char *line;
    char **args;

    while(1) {
        printf("%s > ", cwd);

        line = read_line();

        args = parse_args(line);

        if(strcmp("exit", args[0]) == 0) {
            break;
        } else {
            execute_command(args);
        }

        free(line);
        free(args);
    }
}

void print_welcome() {
    struct passwd *p = getpwuid(getuid());
    printf("Welcome: %s!\n", p->pw_name);
}

int main(int argc, char **argv) {
    print_welcome();

    //TODO What if cwd exceeds size
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        exit(-1);
    }

    char **env = load_profile(cwd);


    for(int i = 0; i < sizeof(env) / sizeof(char*); i++) {
        printf("%s", env[i]);
    }

    command_loop(cwd);

    printf("Thanks, come again\n");

    return 0;
}
