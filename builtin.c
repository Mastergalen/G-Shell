#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "definition.h"
#include "loadProfile.h"

/**
 * Change the cwd of the shell
 * @param  shell Shell state
 * @param  path  Path to change to
 * @return       Success status
 */
int cd(Shell *shell, const char *path) {
    if(path == NULL) {
        path = shell->home;
    }

    if(chdir(path) == -1) {
        printf("Tried changing to %s\n", path);
        perror("Error changing dir");
        return -1;
    };

    if (getcwd(shell->cwd, sizeof(shell->cwd)) == NULL) {
        perror("getcwd() error");
        exit(-1);
    }

    return 1;
}

/**
 * Handles the builtin command for environment variable
 * assignments
 * @param  shell Shell state
 * @param  input Full variable assignment string. E.g. $HOME=/home
 * @return       Success status
 */
int set_shell_variable(Shell *shell, const char *input) {
    set_variable(shell, input);

    return 1;
}
