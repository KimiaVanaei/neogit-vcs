#include "file_functions.h"

void makeHEADzero_afterchkot_byID()
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
		if (strncmp(line, "HEAD(1 means u are in HEAD)", 27) == 0)
		{
			fprintf(tmp_file, "HEAD(1 means u are in HEAD) %d\n", 0);
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


void makeHEADone_aftergoing_head()
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
	if (file == NULL) {
		return;
	}
		
	char tmpconfPath[PATH_MAX];
	snprintf(tmpconfPath, sizeof(tmpconfPath), "%s/.neogit/tmp_config", currentDir);
	FILE *tmp_file = fopen(tmpconfPath, "w");
	if (tmp_file == NULL)
		return;

	char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
	{
		if (strncmp(line, "HEAD(1 means u are in HEAD)", 27) == 0)
		{
			fprintf(tmp_file, "HEAD(1 means u are in HEAD) %d\n", 1);
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


int HEADorNOT()
{
    char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return -1;

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

	if (foundNeogit)
	{
		
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return -1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return -1;
        char confPath[PATH_MAX];
        snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
        FILE *file = fopen(confPath, "r");
        if (file == NULL)
            return -1;
            int HEAD;
        
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (strncmp(line, "HEAD(1 means u are in HEAD)", 27) == 0)
            {
                sscanf(line, "HEAD(1 means u are in HEAD) %d\n", &HEAD);
            }
        }
        fclose(file);
        return HEAD;
    }

}