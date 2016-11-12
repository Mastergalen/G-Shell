#include <unistd.h>
#include <sys/wait.h>

/**
 * Executes a command by spawning a new process
 * @param  command Command to execute
 * @return      Return status
 */
int execute_cmd(char **args) {
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
