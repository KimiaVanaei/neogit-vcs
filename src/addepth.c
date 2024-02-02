#include "file_functions.h"

int isStaged(const char *filename)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    char originalCwd[1024];
    strcpy(originalCwd, cwd);

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening current directory");
            return 0;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
            {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 0;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 0;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit)
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return 0;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return 0;
        char stgPath[PATH_MAX];
        snprintf(stgPath, sizeof(stgPath), "%s/.neogit/staging", currentDir);
        FILE *stagingFile = fopen(stgPath, "r");
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
        if (chdir(originalCwd) != 0)
        {
            perror("Error restoring original directory");
            return 1;
        }
        fclose(stagingFile);
    }
    return 0; // Not staged
}

int add_depth(const char *currentDir, int depth)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    char originalCwd[1024];
    strcpy(originalCwd, cwd);

    char tmp_cwd[1024];

    bool foundNeogit = false;
    struct dirent *entry1;

    do
    {
        DIR *dir1 = opendir(".");
        if (dir1 == NULL)
        {
            perror("Error opening current directory");
            return 0;
        }

        while ((entry1 = readdir(dir1)) != NULL)
        {
            if (entry1->d_type == DT_DIR && strcmp(entry1->d_name, ".neogit") == 0)
            {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir1);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 0;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 0;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return 0;
    }
    if (chdir(originalCwd) != 0)
    {
        perror("Error restoring original directory");
        return 0;
    }
    int set = configs_are_set();
    if (!set)
    {
        printf("Please set local or global configs first\n");
        return 0;
    }
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