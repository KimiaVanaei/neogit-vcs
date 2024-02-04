#include "file_functions.h"
char *currentBranch()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return NULL;
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
        printf("neogit is not initialized\n");
        return NULL;
    }
    if (chdir(tmp_cwd) != 0)
    {
        perror("Error changing directory");
        return NULL;
    }
    strcpy(currentDir, tmp_cwd);
    if (chdir(cwd) != 0)
        return NULL;
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    char *branch = (char *)malloc(50 * sizeof(char));
    FILE *file = fopen(confPath, "r");
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
    if (chdir(originalCwd) != 0)
		{
			perror("Error restoring original directory");
			return NULL;
		}
    return branch;
    fclose(file);
}

char *currentAuthor()
{
    char cwd[1024];
    char originalCwd[1024];
    strcpy(originalCwd, cwd);
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return NULL;

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
        printf("neogit is not initialized\n");
        return NULL;
    }
    if (chdir(tmp_cwd) != 0)
    {
        perror("Error changing directory");
        return NULL;
    }
    strcpy(currentDir, tmp_cwd);
    if (chdir(cwd) != 0)
        return NULL;
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    char *localauthor = (char *)malloc(50 * sizeof(char));
    char *globauthor = (char *)malloc(50 * sizeof(char));

    long int time_local;
    long int time_glob;

    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return NULL;

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "local username", 14) == 0)
        {
            sscanf(line, "local username: %s\t%ld\n", localauthor, &time_local);
        }
    }
    fclose(file);

    FILE *file2 = fopen("/home/kimia/.neogitconfig/info.txt", "r");
    if (file2 == NULL)
        return NULL;
    while (fgets(line, sizeof(line), file2) != NULL)
    {
        if (strncmp(line, "username", 8) == 0)
        {
            sscanf(line, "username: %s\t%ld\n", globauthor, &time_glob);
        }
    }
    fclose(file2);
    // if (chdir(originalCwd) != 0)
	// 	{
	// 		perror("Error restoring original directory");
	// 		return NULL;
	// 	}

    if (time_glob > time_local)
    {
        return globauthor;
    }
    else
    {
        return localauthor;
    }
}