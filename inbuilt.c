#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "definition.h"
#include "loadProfile.h"

/**
 * Change the cwd of the shell
 * @param  shell [description]
 * @param  path  Path to change to
 * @return       Success status
 */
int cd(Shell *shell, char *path) {
    if(path == NULL) {
        path = shell->home;
    }

    if(chdir(path) == -1) {
        perror("Error changing dir");
        return -1;
    };

    if (getcwd(shell->cwd, sizeof(shell->cwd)) == NULL) {
        perror("getcwd() error");
        exit(-1);
    }

    return 1;
}

int set_shell_variable(Shell *shell, char *input) {
    set_variable(shell, input);

    return 1;
}
