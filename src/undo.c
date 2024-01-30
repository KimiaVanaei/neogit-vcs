#include "file_functions.h"

int printlastline() {
   char last_line[1024];
   char *path;

    // Get the absolute path of the current working directory
    path = realpath(".", NULL);

    if (path == NULL) {
        perror("Error getting current working directory");
        return EXIT_FAILURE;
        free(path);
    }
    strcat(path, "/");

FILE *fp = fopen(".neogit/staging", "r");

while(!feof(fp))
    fgets(last_line, 1024, fp);
    char* startPtr = strstr(last_line, path);
    if (startPtr == NULL) {
        // Starting token not found
        return 1;
    }
    startPtr += strlen(path);

    // Find the ending position of the word
    char* endPtr = strstr(startPtr, "/");
    if (endPtr == NULL) {
        // Ending token not found
        return 1;
    }

    // Calculate the length of the word
    size_t wordLength = endPtr - startPtr;

    // Allocate memory for the word and copy it
    char* extractedWord = (char*)malloc(wordLength + 1);
    strncpy(extractedWord, startPtr, wordLength);
    extractedWord[wordLength] = '\0'; // Null-terminate the strin
    printf("%s\n", extractedWord);

    

    return 0;
}
