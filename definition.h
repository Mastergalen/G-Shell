typedef struct Shell {
    char cwd[1024];

    char **path;
    char *home;
} Shell;
