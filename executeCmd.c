#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>

#include "definition.h"

/**
 * Executes a command by spawning a new process
 * @param  command Command to execute
 * @return      Return status
 */
int execute_cmd(Shell *shell, char **args) {
    pid_t pid, waitingPid;
    int status;

    char *pathPrefix = "PATH=";
    char path[strlen(pathPrefix) + strlen(shell->path) + 1];
    strcpy(path, pathPrefix);
    strcat(path, shell->path);

    char *homePrefix = "HOME=";
    char home[strlen(homePrefix) + strlen(shell->home) + 1];
    strcpy(home, homePrefix);
    strcat(home, shell->home);

    char *env_args[] = { "PATH=/bin", home, NULL };

    printf("Loaded path: %s\n", env_args[0]);
    printf("Loaded home: %s\n", env_args[1]);

    pid = fork();

    if(pid < 0) {
        perror("Error forking process");
    } else if (pid == 0) {
        //child process, execute the command

        if(execve(args[0], &args[0], env_args) == -1) {
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
