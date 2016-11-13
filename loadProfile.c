#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "definition.h"

char **split_line(char *line) {
    int size = 2;

    char **pieces = malloc(sizeof(char*) * size);
    char *piece;

    if(!pieces) {
        perror("malloc error");
        exit(-1);
    }

    piece = strtok(line, "=");

    for(int i = 0; i < size; i++) {
        pieces[i] = piece;

        piece = strtok(NULL, "=");
    }

    return pieces;
}

char **split_path(char *paths) {
    
}

void load_profile(char *profileLocation, Shell *shell) {
    FILE *file;
    size_t read;
    size_t len = 0;
    char *line = NULL;

    //TODO What if path does not have enough memory
    strcat(profileLocation, "/profile");

    file = fopen(profileLocation, "r");

    if(file == NULL) {
        perror("Failed reading profile");
        exit(-1);
    }

    while((read = getline(&line, &len, file)) != -1) {
        line[strlen(line) - 1] = '\0'; //Remove \n char

        char **pieces = split_line(line);

        if(strcmp(pieces[0], "HOME") == 0) {
            printf("Setting home to %s\n", pieces[1]);
            shell->home = strdup(pieces[1]);
        } else if(strcmp(pieces[0], "PATH") == 0) {
            shell->path = strdup(pieces[1]);
            printf("Set path to %s\n", shell->path);
        }

        free(pieces);
    }

    fclose(file);

    free(line);
}
