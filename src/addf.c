#include "file_functions.h"

int run_add_single(char *path)
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
        struct stat file_stat;
        if (stat(path, &file_stat) != 0)
        {
            return -1;
        }

        if (S_ISDIR(file_stat.st_mode))
        {
            return add_to_staging_recursive(path);
        }
        else if (S_ISREG(file_stat.st_mode))
        {
            return add_to_staging(path);
        }

        return add_to_staging(path);
    }
}