#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#include "definition.h"

/**
 * Change the cwd of the shell
 * @param  shell [description]
 * @param  path  Path to change to
 * @return       Success status
 */
int cd(Shell *shell, char *path) {
    if(path == NULL) {
        printf("Changing to HOME\n");
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

void set_variable(Shell *shell, char *var, char *value) {

}
