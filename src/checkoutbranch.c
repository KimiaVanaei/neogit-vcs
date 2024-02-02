#include "file_functions.h"

void change_branch_in_configs(char *new_branchname)
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
		if (strncmp(line, "branch", 6) == 0)
		{
			fprintf(tmp_file, "branch: %s\n", new_branchname);
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

int run_checkouttobranch(char *branchname)
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

		char neoPath[PATH_MAX];
		snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
		FILE *stgFile = fopen(neoPath, "r");
		if (stgFile == NULL)
			return 1;
		fseek(stgFile, 0, SEEK_END);
		long size = ftell(stgFile);
		rewind(stgFile);
		if (size != 0)
		{
			printf("You have uncommitted files. You can`t checkout\n");
			return 1;
		}
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
		makeHEADone_aftergoing_head();

		return 0;
	}
}

int checkout_file_to_branch(char *filename, char *filepath, char *branchname)
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

		char filPath[PATH_MAX];
		snprintf(filPath, sizeof(filPath), "%s/.neogit/branches/", currentDir);
		char confPath[PATH_MAX];
		snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
		FILE *file = fopen(confPath, "r");
		if (file == NULL)
			return -1;

		int last_commit_ID_of_branch;
		char line[1024];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strncmp(line, branchname, strlen(branchname)) == 0)
			{
				sscanf(line, "%*s %d\n", &last_commit_ID_of_branch);
			}
		}
		fclose(file);

		strcat(filPath, branchname);
		char tmp[10];
		sprintf(tmp, "/%d", last_commit_ID_of_branch);

		char src_file[1024];
		strcpy(src_file, filPath);
		strcat(src_file, tmp);
		strcat(src_file, "/");
		strcat(src_file, filename);

		FILE *read_file = fopen(src_file, "r");
		if (read_file == NULL)
			return 1;
		FILE *write_file = fopen(filepath, "w");
		if (write_file == NULL)
			return 1;
		char line2[1024];
		while (fgets(line2, sizeof(line2), read_file) != NULL)
		{
			fprintf(write_file, "%s", line2);
		}
		fclose(read_file);
		fclose(write_file);
		return 0;
	}
}

// if the branchname is master:
int run_checkout_master(int argc, char *const argv[])
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
		char neoPath[PATH_MAX];
		snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
		FILE *stgFile = fopen(neoPath, "r");
		if (stgFile == NULL)
		{
			return 1;
		}
		fseek(stgFile, 0, SEEK_END);
		long size = ftell(stgFile);
		rewind(stgFile);
		if (size != 0)
		{
			printf(RED "You have uncommitted files. You can`t checkout\n" RESET);
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
		makeHEADone_aftergoing_head();

		return 0;
	}
}
