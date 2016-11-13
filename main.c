#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "definition.h"
#include "executeCmd.h"
#include "loadProfile.h"
#include "inputHandler.h"

/**
 * G-Shell by Galen Han
 */

void command_loop(Shell *shell) {
    char *line;
    char **args;

    while(1) {
        printf("%s > ", shell->cwd);

        line = read_line();

        args = parse_args(line);

        if(strcmp("exit", args[0]) == 0) {
            break;
        } else if(args[0] == NULL) {
            //Empty command entered, continue
            continue;
        } else {
            execute_cmd(shell, args);
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
    Shell shell;

    if (getcwd(shell.cwd, sizeof(shell.cwd)) == NULL) {
        perror("getcwd() error");
        exit(-1);
    }

    load_profile(shell.cwd, &shell);

    command_loop(&shell);

    printf("Thanks, come again\n");

    return 0;
}
