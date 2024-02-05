#include "file_functions.h"
int extract_current_ID()
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
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return -1;

    int last_commit_ID_total;
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "current_commit_ID", 17) == 0)
        {
            sscanf(line, "current_commit_ID: %d\n", &last_commit_ID_total);
        }
    }
    return last_commit_ID_total;
}

void change_current_ID(int new_currentID)
{
    char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return;

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

	if (!foundNeogit)
	{
		printf("neogit has not initialized\n");
		return;
	}
	if (chdir(tmp_cwd) != 0)
	{
		perror("Error changing directory");
		return;
	}
	strcpy(currentDir, tmp_cwd);
	if (chdir(cwd) != 0)
		return;
	char confPath[PATH_MAX];
	snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
	FILE *file = fopen(confPath, "r");
	if (file == NULL)
		return;
	char tmpconfPath[PATH_MAX];
	snprintf(tmpconfPath, sizeof(tmpconfPath), "%s/.neogit/tmp_config", currentDir);
	FILE *tmp_file = fopen(tmpconfPath, "w");
	if (tmp_file == NULL)
		return;

	char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
	{
		if (strncmp(line, "current_commit_ID", 17) == 0)
		{
			fprintf(tmp_file, "current_commit_ID: %d\n", new_currentID);
		}
		else
		{
			fprintf(tmp_file, "%s", line);
		}
	}
	fclose(file);
	fclose(tmp_file);
	remove(confPath);
	rename(tmpconfPath, confPath);
}

int extract_lastID_master()
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
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return -1;

    int last_commit_ID_total;
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "last_commit_ID", 14) == 0)
        {
            sscanf(line, "last_commit_ID: %d\n", &last_commit_ID_total);
        }
    }
    return last_commit_ID_total;
}

int extract_lastID_branch(char *branchname)
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
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return -1;

    int last_commit_ID_total;
    char line[1024];
   while (fgets(line, sizeof(line), file) != NULL)
	{
			if (strncmp(line, branchname, strlen(branchname)) == 0)
			{
               sscanf(line, "%*s %d\n", &last_commit_ID_total);
            }
    }
    return last_commit_ID_total;
}

int extract_lastID_total()
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
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return -1;

    int last_commit_ID_total;
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "last_id_total", 13) == 0)
        {
            sscanf(line, "last_id_total: %d\n", &last_commit_ID_total);
        }
    }
    fclose(file);
    return last_commit_ID_total;
}