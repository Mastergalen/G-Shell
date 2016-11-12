#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "executeCmd.h"
#include "loadProfile.h"
#include "inputHandler.h"

void command_loop(char cwd[]) {
    char *line;
    char **args;

    while(1) {
        printf("%s > ", cwd);

        line = read_line();

        args = parse_args(line);

        if(strcmp("exit", args[0]) == 0) {
            break;
        } else {
            execute_cmd(args);
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
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        exit(-1);
    }

    char **env = load_profile(cwd);


    for(int i = 0; i < sizeof(env) / sizeof(char*); i++) {
        printf("%s", env[i]);
    }

    command_loop(cwd);

    printf("Thanks, come again\n");

    return 0;
}
