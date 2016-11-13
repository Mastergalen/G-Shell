#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "definition.h"

#define PATH_BUFFER_SIZE 64
#define PATH_DELIM ":"

/**
 * Splits the string for variable assignment into two
 * pieces at the '=' character
 * @param  line string input
 * @return      Array of strings
 */
char **split_assignment(char *line) {
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

/**
 * Splits a path string into pieces
 * E.g. "$PATH=/bin:/usr/bin"
 * Becomes ["/bin", "/usr/bin"]
 * @param  string Path string
 * @return        Array of strings, paths to folders
 */
char **split_path(char *string) {
    int bufferSize = PATH_BUFFER_SIZE;

    char **paths = malloc(sizeof(char*) * bufferSize);
    char *path;

    if(!paths) {
        perror("Memory allocation error");
        exit(-1);
    }

    path = strtok(string, PATH_DELIM);

    int i = 0;
    while(path != NULL) {
        paths[i] = path;
        i++;

        if(i > bufferSize) {
            bufferSize += PATH_BUFFER_SIZE;

            paths = realloc(paths, sizeof(char*) * bufferSize);

            if(!paths){
                perror("Memory reallocation error");
                exit(-1);
            }
        }

        path = strtok(NULL, PATH_DELIM);
    }

    paths[i] = NULL;

    return paths;
}

/**
 * Sets the shell state environment variables
 * E.g. "$HOME=/home/os" sets shell->home to "/home/os"
 * @param shell Shell state
 * @param line  Line string
 */
void set_variable(Shell *shell, char *line) {
    char **pieces = split_assignment(line);

    //Ignore dollar sign
    if(pieces[0][0] == '$') pieces[0]++;

    if(strcmp(pieces[0], "HOME") == 0) {
        shell->home = strdup(pieces[1]);
    } else if(strcmp(pieces[0], "PATH") == 0) {
        shell->path = split_path(pieces[1]);
    } else {
        printf("Unable to set %s\n", pieces[0]);
    }

    free(pieces);
}

/**
 * Loads the profile and updates the shell state
 * @param profileLocation Path to profile folder
 * @param shell           Shell state
 */
void load_profile(char *profileLocation, Shell *shell) {
    FILE *file;
    size_t read;
    size_t len = 0;
    char *line = NULL;

    char *profilePath = strdup(profileLocation);
    strcat(profilePath, "/profile");

    file = fopen(profilePath, "r");

    if(file == NULL) {
        perror("Failed reading profile");
        exit(-1);
    }

    while((read = getline(&line, &len, file)) != -1) {
        line[strlen(line) - 1] = '\0'; //Remove \n char

        set_variable(shell, line);
    }

    fclose(file);

    free(line);

    //TODO Check if error is thrown
    if(shell->home == NULL) {
        perror("Error: HOME not set in profile!");
        exit(-1);
    }

    if(shell->path == NULL) {
        perror("Error: PATH not set in profile!");
        exit(-1);
    }
}
