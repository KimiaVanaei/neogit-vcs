#include "file_functions.h"
char *currentBranch()
{
    char *branch = (char*)malloc(50 * sizeof(char));
    FILE *file = fopen(".neogit/config", "r");
    if (file == NULL)
        return NULL;

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "branch", 6) == 0)
        {
            sscanf(line, "branch: %s\n", branch);
        }
    }
    return branch;
    fclose(file);
}

char *currentAuthor()
{
    char *author = (char*)malloc(50 * sizeof(char));
    FILE *file = fopen(".neogit/config", "r");
    if (file == NULL)
        return NULL;

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "local username", 14) == 0)
        {
            sscanf(line, "local username: %s\n", author);
        }
    }
    if (strcmp(author, "_") != 0)
    {
        fclose(file);
        return author;
    }
    else
    {
        fclose(file);
        FILE *file2 = fopen("/home/kimia/.neogitconfig/info.txt", "r");
        if (file2 == NULL)
            return NULL;
        while (fgets(line, sizeof(line), file2) != NULL)
        {
            if (strncmp(line, "username", 8) == 0)
            {
                sscanf(line, "username: %s\n", author);
            }
        }
        fclose(file2);
        return author;
    }
}