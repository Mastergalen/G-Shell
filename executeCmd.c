#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "definition.h"

/**
 * Returns the full path of a program if it exists in one of the
 * path folders
 * @param  path    Array of strings of folders
 * @param  program Name of the program to search for
 * @return         Full path to the program, otherwise NULL
 */
char *find_program(char **path, char *program) {
    struct stat buffer;
    int i = 0;

    char *fullPath = malloc(sizeof(char) * (4096 + 1 + 1));

    while(path[i] != NULL) {
        strcpy(fullPath, path[i]);
        strcat(fullPath, "/");
        strcat(fullPath, program);

        if(stat(fullPath, &buffer) == 0) {
            //File exists
            return fullPath;
        }
        i++;
    }

    //Did not find file, return NULL
    return NULL;
}

/**
 * Executes a command by spawning a new process
 * @param  command Command to execute
 * @return      Return status
 */
int execute_cmd(Shell *shell, char **args) {
    pid_t pid, waitingPid;
    int status;

    char *homePrefix = "HOME=";
    char home[strlen(homePrefix) + strlen(shell->home) + 1];
    strcpy(home, homePrefix);
    strcat(home, shell->home);

    //printf("Loaded home: %s\n", env_args[1]);

    pid = fork();

    if(pid < 0) {
        perror("Error forking process");
    } else if (pid == 0) {
        //child process, execute the command

        char *programPath = find_program(shell->path, args[0]);

        if(programPath == NULL) {
            perror("Could not find the command");
        } else if(execv(programPath, args) == -1) {
            perror("Error executing command");
        }

    } else {
        //Parent process, wait for child process
        do {
            waitingPid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
