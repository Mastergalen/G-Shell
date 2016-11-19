#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "definition.h"
#include "builtin.h"
#include "utils.h"

/**
 * Returns the full path of a program if it exists in one of the
 * path folders
 * @param  path    Array of strings of folders
 * @param  program Name of the program to search for
 * @return         Full path to the program, otherwise NULL
 */
char *find_program(char **path, const char *program) {
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
int execute_cmd(Shell *shell, const char **args) {

    if(strcmp(args[0], "cd") == 0) {
        return cd(shell, args[1]);
    } else if(args[0][0] == '$') {
        return set_shell_variable(shell, args[0]);
    }

    pid_t pid;
    int status;

    char **argsCopy = str_array_dup(args);

    pid = fork();

    if(pid < 0) {
        perror("Error forking process");
    } else if (pid == 0) {
        //child process, execute the command

        char *programPath = find_program(shell->path, argsCopy[0]);

        if(programPath == NULL) {
            perror("Could not find the command");
        } else if(execv(programPath, argsCopy) == -1) {
            perror("Error executing command");
        }

        free(programPath);
    } else {
        //Parent process, wait for child process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        //WIFEXITED = "wife exited" lol
    }

    free_str_array(argsCopy);

    return 1;
}
