#include "file_functions.h"
int run_checkoutHEAD(int argc, char *const argv[])
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

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");

        return 1;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return 1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return 1;

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
            if (strncmp(line, "last_commit_ID", 14) == 0)
            {
                sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
            }
        }
        fclose(file);

        if (chdir(originalCwd) != 0)
        {
            perror("Error restoring original directory");
            return 1;
        }

        DIR *dir = opendir(".");
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                char filePath[PATH_MAX];
                if (realpath(entry->d_name, filePath) == NULL)
                {
                    perror("realpath");
                    exit(EXIT_FAILURE);
                }

                if (is_tracked(filePath))
                    checkout_file(entry->d_name, find_file_last_change_before_commit(entry->d_name, last_commit_ID), filePath);
            }
        }
        closedir(dir);
        

        return 0;
    }
}

int run_checkoutHEAD_forbr(char *branchname)
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

	if (!foundNeogit)
	{
		printf("neogit has not been initialized\n");

		return 1;
	}
	else
	{
		if (chdir(tmp_cwd) != 0)
		{
			perror("Error changing directory");
			return 1;
		}
		strcpy(currentDir, tmp_cwd);
		if (chdir(cwd) != 0)
			return 1;

		if (chdir(originalCwd) != 0)
		{
			perror("Error restoring original directory");
			return 1;
		}

		DIR *dir = opendir(".");
		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_REG)
			{
				char filePath[PATH_MAX];
				if (realpath(entry->d_name, filePath) == NULL)
				{
					perror("realpath");
					exit(EXIT_FAILURE);
				}

				if (is_tracked(filePath))
					checkout_file_to_branch(entry->d_name, filePath, branchname);
			}
		}
		closedir(dir);
         
		return 0;
	}
}
