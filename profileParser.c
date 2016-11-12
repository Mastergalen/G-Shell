#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define LINE_BUFFER_SIZE 1024
#define ENV_BUFFER_SIZE 64

char **load_profile(char *path) {
    FILE *file;
    size_t read;
    size_t len = 0;
    char *line;

    int bufferSize = ENV_BUFFER_SIZE;
    char **envVars = malloc(sizeof(char*) * bufferSize);

    //TODO What if path does not have enough memory
    strcat(path, "/profile");

    file = fopen(path, "r");

    if(file == NULL) {
        perror("Failed reading profile");
        exit(-1);
    }

    int i = 0;
    while((read = getline(&line, &len, file)) != -1) {
        printf("%s", line);
        envVars[i] = line;
        i++;

        if(i > bufferSize) {
            bufferSize += ENV_BUFFER_SIZE;

            envVars = realloc(envVars, sizeof(char*) * bufferSize);

            if(!envVars){
                perror("EnvVars reallocation error");
                exit(-1);
            }
        }
    }

    fclose(file);

    free(line);

    return envVars;
}
