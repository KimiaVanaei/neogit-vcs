#include "file_functions.h"
void identifyWordsWithWildcard(char *pattern) {
    glob_t globResult;

    if (glob(pattern, 0, NULL, &globResult) == 0) {
        printf("Matching words:\n");
        for (size_t i = 0; i < globResult.gl_pathc; ++i) {
            printf("%s\n", globResult.gl_pathv[i]);
        }
        
        // Free the memory allocated by glob
        globfree(&globResult);
    } else {
        printf("No matching words found.\n");
    }
}

int containsWildcard(char *str) {
    while (*str) {
        if (*str == '*') {
            return 1;  // Wildcard found
        }
        str++;
    }
    return 0;  // No wildcard found
}