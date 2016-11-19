#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Concatenates two strings
 * @param  s1 String 1
 * @param  s2 String 2
 * @return    Merged string
 */
char *str_concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);

    if(result == NULL) {
        perror("Malloc error");
        exit(-1);
    }

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

/**
 * Frees memory for array of strings
 * @param array Pointer to string array
 */
void free_str_array(char **array) {
    int i = 0;

    while(array[i] != NULL) {
        free(array[i]);
        i++;
    }

    free(array);
}

char **str_array_dup(const char **src) {

    int length = 0; //Start with 1 to account for null
    while(src[length] != NULL) {
        length++;
    }
    length++; //One more for NULL at the end

    char **dup = malloc(sizeof(char*) * length);

    int i = 0;
    while(src[i] != NULL) {
        int width = strlen(src[i]) + 1;
        dup[i] = malloc(width);
        memcpy(dup[i], src[i], width);
        i++;
    }
    dup[i] = NULL;

    return dup;
}
