#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definition.h"
#include "utils.h"

#define PATH_BUFFER_SIZE 64
#define PATH_DELIM ":"

/**
 * Splits the string for variable assignment into two
 * pieces at the '=' character
 * @param  line string input
 * @return      Array of strings
 */
char **split_assignment(const char *line) {
    int size = 3;

    char **pieces = malloc(sizeof(char*) * size);
    char *piece;

    if(!pieces) {
        perror("malloc error");
        exit(-1);
    }

    char *lineCopy = strdup(line);
    piece = strtok(lineCopy, "=");

    int i = 0;
    while(piece != NULL && i < 2) {
        pieces[i] = strdup(piece);

        piece = strtok(NULL, "=");
        i++;
    }

    pieces[i] = NULL;

    free(lineCopy);
    free(piece);

    return pieces;
}

/**
 * Splits a path string into pieces
 * E.g. "$PATH=/bin:/usr/bin"
 * Becomes ["/bin", "/usr/bin"]
 * @param  string Path string
 * @return        Array of strings, paths to folders
 */
char **split_path(const char *string) {
    int bufferSize = PATH_BUFFER_SIZE;

    char **paths = malloc(sizeof(char*) * bufferSize);
    char *path;

    if(!paths) {
        perror("Memory allocation error");
        exit(-1);
    }

    char *stringCopy = strdup(string);

    path = strtok(stringCopy, PATH_DELIM);

    int i = 0;
    while(path != NULL) {
        paths[i] = strdup(path);
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

    free(stringCopy);
    free(path);

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

    if(pieces[1] == NULL) {
        printf("Error: Invalid assignment value\n");
        free_str_array(pieces);
        return;
    }

    //Ignore dollar sign for variavle
    char *var = pieces[0];
    if(var[0] == '$') var++;

    if(strcmp(var, "HOME") == 0) {
        shell->home = strdup(pieces[1]);
    } else if(strcmp(var, "PATH") == 0) {
        shell->path = split_path(pieces[1]);
    } else {
        printf("Unable to set %s\n", var);
    }

    free_str_array(pieces);
}

/**
 * Loads the profile and updates the shell state
 * @param profileLocation Path to profile folder
 * @param shell           Shell state
 */
void load_profile(const char *profileLocation, Shell *shell) {
    FILE *file;
    size_t read;
    size_t len = 0;
    char *line = NULL;

    char *profilePath = str_concat(profileLocation, "/profile");

    file = fopen(profilePath, "r");

    free(profilePath);

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

    if(shell->home == NULL || strlen(shell->home) == 0) {
        printf("Error: HOME not set in profile!\n");
        exit(-1);
    }

    if(shell->path == NULL) {
        printf("Error: PATH not set in profile!\n");
        exit(-1);
    }
}
