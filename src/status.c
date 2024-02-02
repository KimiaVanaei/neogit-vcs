#include "file_functions.h"

int run_status()
{
    DIR *dir;
    struct dirent *entry;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    dir = opendir(".");
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;
    }
    char current_directory[PATH_MAX];

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (entry->d_type == DT_REG)
        {
            char filePath[PATH_MAX];
            strcpy(filePath, cwd);
            strcat(filePath, "/");
            strcat(filePath, entry->d_name);

            struct stat file_info;
            if (stat(filePath, &file_info) != 0)
            {
                perror("Error getting file information");
                return 1;
            }
            time_t modification_time = file_info.st_mtime;

            int last_commit_ID = last_ID();
            if (last_commit_ID == 0)
            {
                    if (isStaged(filePath))
                    {
                        long int savedModTime_instg1 = getSavedModTime_fromstg(filePath);
                        printf("%ld\n", savedModTime_instg1);
                        printf("%ld\n", modification_time);
                        if (savedModTime_instg1 != modification_time)
                        {

                            printf("%s\t%c%c\n", entry->d_name, '+', 'M');
                        }
                        else
                        {
                           printf("%s\t%c%c\n", entry->d_name, '+', 'A');
                        }
                    }
                    else {
                        printf("%s\t%c%c\n", entry->d_name, '-', 'A');
                    }
            }
            else
            {

                int res = isInLastCommits(filePath, last_commit_ID);

                if (res == -1)
                    return 0;
                else if (res == 0) // dar akharin commit nist
                {
                    
                    if (isStaged(filePath))
                    {
                        long int savedModTime_instg = getSavedModTime_fromstg(filePath);
                        if (savedModTime_instg != modification_time)
                        {
                            printf("%s\t%c%c\n", entry->d_name, '+', 'M');
                        }
                        else
                        {
                             printf("%ld\n", savedModTime_instg);
                        printf("%ld\n", modification_time);
                            // ?????
                        }
                    }

                    else
                        printf("%s\t%c%c\n", entry->d_name, '-', 'A');
                }
                else if (res == 1)
                {
                    long int savedModTime = getSavedModTime(filePath, last_commit_ID);

                    if (savedModTime != modification_time)
                    {
                        if (isStaged(filePath))
                            printf("%s\t%c%c\n", entry->d_name, '+', 'M');
                        else
                            printf("%s\t%c%c\n", entry->d_name, '-', 'M');
                    }
                }

            }
        }
    }
                char *dd = (char *)malloc((PATH_MAX) * sizeof(char));
                dd = isInLastCommitsButNotInCurr();
                if (dd != NULL)
                {
                    char *fileName = strrchr(dd, '/');

                    if (fileName != NULL)
                    {
                        fileName++;
                    }
                    if (isStaged(dd))
                        printf("%s\t%c%c\n", fileName, '+', 'D');
                    else
                        printf("%s\t%c%c\n", fileName, '-', 'D');
                }
    return 0;
}

int isInLastCommits(char *filepath, int last_ID)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return -1;
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
            return -1;
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
                return -1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");

        return -1;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return -1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return -1;

        int found = 0;
        char lastcommit_path[PATH_MAX];
        snprintf(lastcommit_path, sizeof(lastcommit_path), "%s/.neogit/commits/", currentDir);

        char tmp[10];
        sprintf(tmp, "%d", last_ID);
        strcat(lastcommit_path, tmp);
        FILE *fptr = fopen(lastcommit_path, "r");
        char line[1024];
        while (fgets(line, sizeof(line), fptr) != NULL)
        {
            int length = strlen(line);

            if (length > 0 && line[length - 1] == '\n')
            {
                line[length - 1] = '\0';
            }
            char stored_path[1024];
            sscanf(line, "%s", stored_path);

            if (strcmp(stored_path, filepath) == 0)
            {
                found = 1;
                fclose(fptr);
                return found;
            }
        }
        fclose(fptr);
        if (chdir(originalCwd) != 0)
        {
            perror("Error restoring original directory");
            return -1;
        }
        return found;
    }
}
long int getSavedModTime(char *filepath, int last_ID)
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

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");

        return 0;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return 0;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return 0;

        char lastcommit_path[PATH_MAX];
        snprintf(lastcommit_path, sizeof(lastcommit_path), "%s/.neogit/commits/", currentDir);
        char tmp[10];
        sprintf(tmp, "%d", last_ID);
        strcat(lastcommit_path, tmp);
        FILE *fptr = fopen(lastcommit_path, "r");
        char line[1024];
        while (fgets(line, sizeof(line), fptr) != NULL)
        {
            int length = strlen(line);

            if (length > 0 && line[length - 1] == '\n')
            {
                line[length - 1] = '\0';
            }
            char stored_path[1024];
            long int savedModTime;
            if (sscanf(line, "%s %ld", stored_path, &savedModTime) == 2 && strcmp(stored_path, filepath) == 0)
            {
                fclose(fptr);
                if (chdir(originalCwd) != 0)
                {
                    perror("Error restoring original directory");
                    return 1;
                }
                return savedModTime;
            }
        }
    }
}

long int getSavedModTime_fromstg(char *filepath)
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

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");

        return 0;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return 0;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return 0;

        char stg_path[PATH_MAX];
        snprintf(stg_path, sizeof(stg_path), "%s/.neogit/staging", currentDir);

        FILE *fptr = fopen(stg_path, "r");
        char line[1024];
        while (fgets(line, sizeof(line), fptr) != NULL)
        {
            int length = strlen(line);

            if (length > 0 && line[length - 1] == '\n')
            {
                line[length - 1] = '\0';
            }
            char stored_path[1024];
            long int savedModTime;
            if (sscanf(line, "%s %ld", stored_path, &savedModTime) == 2 && strcmp(stored_path, filepath) == 0)
            {
                fclose(fptr);
                if (chdir(originalCwd) != 0)
                {
                    perror("Error restoring original directory");
                    return 1;
                }
                return savedModTime;
            }
        }
    }
}
int last_ID()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return -1;
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
            return -1;
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
            return -1;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return -1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");

        return -1;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return -1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return -1;
        char neoPath[PATH_MAX];
        snprintf(neoPath, sizeof(neoPath), "%s/.neogit/config", currentDir);
        FILE *file = fopen(neoPath, "r");
        if (file == NULL)
        {
            return -1;
        }

        int last_commit_ID;
        char line[1024];
        // find last id to access the most recent commit file
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strncmp(line, "last_commit_ID", 14) == 0)
            {
                sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
            }
        }
        fclose(file);
        if (chdir(originalCwd) != 0)
        {
            perror("Error restoring original directory");
            return -1;
        }
        return last_commit_ID;
    }
}

char *isInLastCommitsButNotInCurr()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Error getting current working directory");
        return NULL;
    }

    char originalCwd[PATH_MAX];
    strcpy(originalCwd, cwd);

    char tmp_cwd[PATH_MAX];
    char currentDir[PATH_MAX];
    bool foundNeogit = false;
    struct dirent *entry;

    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening current directory");
            return NULL;
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
            return NULL;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return NULL;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return NULL;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return NULL;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return NULL;

        int found = 0;
        char lastcommit_path[PATH_MAX];
        int ret = snprintf(lastcommit_path, sizeof(lastcommit_path), "%s/.neogit/commits/", currentDir);
        if (ret < 0)
        {
            abort();
        }
        if (chdir(originalCwd) != 0)
        {
            perror("Error restoring original directory");
            return NULL;
        }
         
        char tmp[10];
        sprintf(tmp, "%d", last_ID());
        strcat(lastcommit_path, tmp);
        FILE *fptr = fopen(lastcommit_path, "r");
        if (fptr == NULL)
        {
            perror("Error opening file");
            return NULL;
        }
         char line[PATH_MAX];
        while (fgets(line, sizeof(line), fptr) != NULL)
        {
            int length = strlen(line);

            if (length > 0 && line[length - 1] == '\n')
            {
                line[length - 1] = '\0';
            }

            char *storedpath = (char *)malloc((PATH_MAX) * sizeof(char));
            sscanf(line, "%s", storedpath);

            // Check if the stored path is not present in the current working directory
            if (access(storedpath, F_OK) != 0)
            {
                return strdup(storedpath); // Return the path
            }
        }

        return NULL;
    }
}