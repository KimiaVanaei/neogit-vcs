#include "file_functions.h"
int configs_are_set()
{
    int set = 0;
    char cwd[1024];
    char originalCwd[1024];
    strcpy(originalCwd, cwd);
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 0;

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
        char confPath[PATH_MAX];
        snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
        char *localauthor = (char *)malloc(50 * sizeof(char));
        char *globauthor = (char *)malloc(50 * sizeof(char));
        char *localemail = (char *)malloc(200 * sizeof(char));
        char *globemail = (char *)malloc(200 * sizeof(char));

        FILE *file = fopen(confPath, "r");
        if (file == NULL)
            return 0;

        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strncmp(line, "local username", 14) == 0)
            {
                sscanf(line, "local username: %s", localauthor);
            }
            if (strncmp(line, "local email", 11) == 0)
            {
                sscanf(line, "local email: %s", localemail);
            }
        }
        fclose(file);

        FILE *file2 = fopen("/home/kimia/.neogitconfig/info.txt", "r");
        if (file2 == NULL)
            return 0;
        while (fgets(line, sizeof(line), file2) != NULL)
        {
            if (strncmp(line, "username", 8) == 0)
            {
                sscanf(line, "username: %s", globauthor);
            }
            if (strncmp(line, "email", 5) == 0)
            {
                sscanf(line, "email: %s", globemail);
            }
        }
        fclose(file2);

        if ((strcmp(localauthor, "_") != 0 && strcmp(localemail, "_") != 0) || ( strcmp(globauthor, "_") != 0 && strcmp(globemail, "_") != 0))
        {
            set = 1;
        }
        return set;
    }
}