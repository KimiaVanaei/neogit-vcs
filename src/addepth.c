#include "file_functions.h"

int isStaged(const char *filename)
{
    FILE *stagingFile = fopen(".neogit/staging", "r");
    if (stagingFile == NULL)
    {
        perror("Error opening staging file");
        return 0;
    }

    char line[1024];
    while (fgets(line, sizeof(line), stagingFile) != NULL)
    {
        int length = strlen(line);

        if (length > 0 && line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }

        // Check if the filename is a substring of the line
        if (strstr(line, filename) != NULL)
        {
            fclose(stagingFile);
            return 1; // Staged
        }
    }

    fclose(stagingFile);
    return 0; // Not staged
}

int add_depth(const char *currentDir, int depth)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(currentDir);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip special entries "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Check if the entry is staged
        if (isStaged(entry->d_name))
        {
            printf("%s/%s: staged\n", currentDir, entry->d_name);
        }
        else
        {
            printf("%s/%s: not staged\n", currentDir, entry->d_name);
        }

        // Check if the entry is a directory and depth is greater than 0
        if (entry->d_type == DT_DIR && depth > 0)
        {
            // Construct the full path for the subdirectory
            char subDirPath[1024];
            snprintf(subDirPath, sizeof(subDirPath), "%s/%s", currentDir, entry->d_name);

            // Recursive call with depth decremented
            add_depth(subDirPath, depth - 1);
        }
    }

    closedir(dir);
    return 0;
}