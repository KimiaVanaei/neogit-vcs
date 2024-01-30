#include "file_functions.h"

int run_status()
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;
    }
    char current_directory[PATH_MAX]; // PATH_MAX is a constant defined in <limits.h>

    // if (getcwd(current_directory, sizeof(current_directory)) != NULL) {
    //     printf("Current directory: %s\n", current_directory);
    // } else {
    //     perror("Error getting current directory");
    //     return 1;
    // }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        if (entry->d_type == DT_REG)
        {
            char filePath[PATH_MAX];
            if (realpath(entry->d_name, filePath) == NULL)
            {
                printf("inv file path: %s\n", filePath);
                perror("realpath");
                exit(EXIT_FAILURE);
            }
            printf("corr file path: %s\n", filePath);

            struct stat file_info;
            if (stat(filePath, &file_info) != 0)
            {
                perror("Error getting file information");
                return 1;
            }
            time_t modification_time = file_info.st_mtime;

            // FILE *file = fopen(".neogit/config", "r");
            // if (file == NULL)
            // {
            //     return -1;
            // }

            // int last_commit_ID;
            // char line[1024];
            // // find last id to access the most recent commit file
            // while (fgets(line, sizeof(line), file) != NULL)
            // {
            //     if (strncmp(line, "last_commit_ID", 14) == 0)
            //     {
            //         sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
            //     }
            // }
            // fclose(file);
            int last_commit_ID = last_ID();

            int res = isInLastCommits(filePath, last_commit_ID);

            if (res == 0)
            {
                printf("added---right now is %ld\n", modification_time);
                printf("%s\t%c\n", entry->d_name, 'A');
                
            }
            else
            {
                long int savedModTime = getSavedModTime(filePath, last_commit_ID);
                
                if (savedModTime != modification_time)
                {
printf("mod ----saved is %ld\n", savedModTime);
                printf("mod-----right now is %ld\n", modification_time);
                
                    printf("%s\t%c\n", entry->d_name, 'M');
            }
            }
        }
    }
    return 0;
}

int isInLastCommits(char *filepath, int last_ID)
{
    int found = 0;
    char lastcommit_path[1024];
    strcpy(lastcommit_path, ".neogit/commits/");
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
    return found;
}
long int getSavedModTime(char *filepath, int last_ID)
{
    char lastcommit_path[1024];
    strcpy(lastcommit_path, ".neogit/commits/");
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
            return savedModTime;
        }
    }
}
int last_ID()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return 1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return 1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
		return 1;
    } else {
        printf("neogit has not been initialized\n");
    }
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
            return last_commit_ID;
}