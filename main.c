#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "definition.h"
#include "executeCmd.h"
#include "loadProfile.h"
#include "inputHandler.h"
#include "utils.h"

/* * * * * * * * * * * * *
 * G-Shell by Galen Han  *
 * * * * * * * * * * * * */

/**
 * Main loop of the shell
 * @param shell Shell state
 */
void command_loop(Shell *shell) {
    char *line;
    char **args;

    while(1) {
        printf("%s > ", shell->cwd);

        line = read_line();
        args = parse_args(line);

        if(args[0] != NULL) {
            if(strcmp("exit", args[0]) == 0) break;

            execute_cmd(shell, (const char**) args);
        }

        //Command finished, or empty command entered

        free(line);
        free_str_array(args);
    }

    free(line);
    free_str_array(args);
}

/**
 * Prints a welcome message with the current user
 */
void print_welcome() {
    struct passwd *p = getpwuid(getuid());
    printf("Welcome: %s!\n", p->pw_name);
}

int main(int argc, char **argv) {
    print_welcome();

    Shell shell;
    shell.path = NULL;
    shell.home = NULL;

    if (getcwd(shell.cwd, sizeof(shell.cwd)) == NULL) {
        perror("getcwd() error");
        exit(-1);
    }

    load_profile(shell.cwd, &shell);

    command_loop(&shell);

    free_str_array(shell.path);
    free(shell.home);

    printf("Thanks, come again\n");

    return 0;
}
