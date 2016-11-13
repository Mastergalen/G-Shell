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

        if(execv(args[0], args) == -1) {
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
