#include "file_functions.h"

bool check_branch_directory_exists(char *filepath)
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
    char brPath[PATH_MAX];
    snprintf(brPath, sizeof(brPath), "%s/.neogit/branches/", currentDir);
    DIR *dir = opendir(brPath);
    struct dirent *entry2;
    if (dir == NULL)
    {
        perror("Error opening current directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, filepath) == 0)
            return true;
    }
    closedir(dir);

    return false;
}

int run_branch(char *branch_name)
{
    if (check_branch_directory_exists(branch_name))
    {
        printf("This branch already exists\n");
        return 1;
    }
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
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
    {
        return -1;
    }

    int last_commit_ID;
    char line[1024];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "last_id_total", 13) == 0)
        {
            sscanf(line, "last_id_total: %d\n", &last_commit_ID);
        }
    }
    fclose(file);
    FILE *file2 = fopen(confPath, "a");
    if (file2 == NULL ) return -1;
    fprintf(file2, "%s %d\n", branch_name, last_commit_ID);
    fclose(file2);

    char brPath[PATH_MAX];
    snprintf(brPath, sizeof(brPath), "%s/.neogit/branches/", currentDir);
    char dir_path[PATH_MAX];
    strcpy(dir_path, brPath);
    strcat(dir_path, branch_name);
    strcat(dir_path, "/");

    if (mkdir(dir_path, 0755) != 0)
        return 1;
    char tmp[10];
    sprintf(tmp, "%d", last_commit_ID);
    strcat(dir_path, tmp);
    if (mkdir(dir_path, 0755) != 0)
        return 1;

    char filPath[PATH_MAX];
    snprintf(filPath, sizeof(filPath), "%s/.neogit/files", currentDir);

    hasFileWithNumber(filPath, last_commit_ID, branch_name);
    return 0;
}

void createDirectory(const char *dirPath)
{
    struct stat st = {0};
    if (stat(dirPath, &st) == -1)
    {
        mkdir(dirPath, 0700);
    }
}

void hasFileWithNumber(const char *dirPath, int targetNumber, char *branchname)
{
    DIR *dir = opendir(dirPath);
    struct dirent *entry;

    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            char folderName[256];
            strcpy(folderName, entry->d_name);

            DIR *subDir;
            struct dirent *subEntry;
            char folderPath[PATH_MAX];
            strcpy(folderPath, dirPath);
            strcat(folderPath, "/");
            strcat(folderPath, folderName);

            subDir = opendir(folderPath);
            if (subDir == NULL)
            {
                printf("Error opening subdirectory\n");
                return;
            }

            while ((subEntry = readdir(subDir)) != NULL)
            {
                if (subEntry->d_type == DT_REG)
                {
                    int fileName = atoi(subEntry->d_name);
                    if (fileName == targetNumber)
                    {
                        char tmp[10];
                        sprintf(tmp, "%d", targetNumber);
                        char desdirPath[PATH_MAX];
                        char dircpy[PATH_MAX];
                        strcpy(dircpy, dirPath);
                        strcpy(desdirPath, dirname(dircpy)); // /mnt/c/c_test/proj/.neogit
                        strcat(desdirPath, "/");
                        char readpath[PATH_MAX];
                        strcpy(readpath, desdirPath); // /mnt/c/c_test/proj/.neogit/
                        strcat(readpath, "files/");
                        strcat(readpath, folderName);
                        strcat(readpath, "/");
                        strcat(readpath, tmp);

                        strcat(desdirPath, "branches/");
                        strcat(desdirPath, branchname);
                        strcat(desdirPath, "/");
                        strcat(desdirPath, tmp);
                        strcat(desdirPath, "/");
                        strcat(desdirPath, folderName);
                        FILE *filetowritein = fopen(desdirPath, "w");
                        if (filetowritein == NULL)
                            return;
                        FILE *filetoreadfrom = fopen(readpath, "r");
                        if (filetoreadfrom == NULL)
                            return;
                        char line[1024];

                        while (fgets(line, sizeof(line), filetoreadfrom) != NULL)
                        {
                            fprintf(filetowritein, "%s", line);
                        }
                        fclose(filetoreadfrom);
                        fclose(filetowritein);

                        break;
                    }
                }
            }

            closedir(subDir);
        }
    }

    closedir(dir);
}

void printBranches()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return;
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
            return;
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
            return;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit)
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return;
    }
    char brPath[PATH_MAX];
    snprintf(brPath, sizeof(brPath), "%s/.neogit/branches/", currentDir);
    DIR *dir = opendir(brPath);
    if (dir == NULL) {
        perror("Error opening current directory");
        return;
    }

    struct dirent *entry2;
    printf("master\n");
    while ((entry2 = readdir(dir)) != NULL) {
        if (entry2->d_type == DT_DIR && strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0) {
            printf("%s\n", entry2->d_name);
        }
    }
    closedir(dir);

    if (chdir(originalCwd) != 0)
    {
        perror("Error restoring original directory");
        return ;
    }
}
