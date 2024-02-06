#include "file_functions.h"
int add_to_staging(char *filepath)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

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
            return 1;
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
            return 1;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit)
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return 1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return 1;
    }
    else
    {
        printf("neogit has not been initialized\n");
        return 1;
    }

    char neoPath[PATH_MAX];
    snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);

    FILE *file = fopen(neoPath, "r+");
    if (file == NULL)
        return 1;
    char line[1024];
    int found = 0;
    struct stat file_info;
            if (stat(filepath, &file_info) != 0)
            {
                perror("Error getting file information");
                return 1;
            }
    time_t modification_time = file_info.st_mtime;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int length = strlen(line);

        if (length > 0 && line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }
        char line1[1024];
        sscanf(line, "%s",line1 );

        if (strcmp(filepath, line1) == 0){
            found = 1;
            break;
        }
       
    }
    if (found) {
            long int savedModTime;
            long int savedTime;
            sscanf(line, "%*s\t%ld\t%ld\n", &savedModTime, &savedTime);
            if (savedModTime != modification_time) {
                 time_t currentTime;
                 time(&currentTime);
                 fseek(file, -strlen(line) - 1, SEEK_CUR);
                 fprintf(file, "%s\t%ld\t%ld\n", filepath, (long)modification_time, (long)currentTime);
                 return 0;
            } else {
                 return 0;
            }
    }
    fclose(file);

    file = fopen(neoPath, "a");
    if (file == NULL)
        return 1;
    time_t currentTime;
    time(&currentTime);

    fprintf(file, "%s\t%ld\t%ld\n", filepath, (long)modification_time, (long)currentTime);

    fclose(file);
    return 0;
}

int add_to_staging_recursive(char *dirpath)
{
    DIR *dir = opendir(dirpath);
    if (dir == NULL)
        return 1;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG) // Regular file
        {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
            add_to_staging(filepath);
        }
        else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char subdirectory[1024];
            snprintf(subdirectory, sizeof(subdirectory), "%s/%s", dirpath, entry->d_name);
            add_to_staging_recursive(subdirectory);
        }
    }

    closedir(dir);
    return 0;
}

int run_add(int argc, char *const argv[])
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening current directory");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    if (chdir(cwd) != 0)
        return 1;

    if (!exists)
    {
        perror("neogit has not been initialized");
        return -1;
    }
    else
    {
        int set = configs_are_set();
        if (!set)
        {
            printf(RED "Please set a local or global config first!\n" RESET);
            return -1;
        }
        struct stat file_stat;
        if (stat(argv[2], &file_stat) != 0)
        {
            perror("error accessing file/directory");
            return -1;
        }

        if (S_ISDIR(file_stat.st_mode))
        {
            // Input is a directory, recursively stage its contents
            char folPath[PATH_MAX];
            if (realpath(argv[2], folPath) == NULL)
            {
                perror("realpath");
                exit(EXIT_FAILURE);
            }
            return add_to_staging_recursive(folPath);
        }
        else if (S_ISREG(file_stat.st_mode))
        {
            // Input is a regular file, stage it
            char filePath[PATH_MAX];
            if (realpath(argv[2], filePath) == NULL)
            {
                perror("realpath");
                exit(EXIT_FAILURE);
            }
            return add_to_staging(filePath);
        }
    }
}
