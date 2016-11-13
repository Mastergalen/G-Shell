typedef struct Shell {
    char cwd[4097]; //Max path length in Linux is 4096

    char **path;
    char *home;
} Shell;
