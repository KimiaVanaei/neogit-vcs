#include "file_functions.h"

int run_checkout(int argc, char *const argv[])
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

		if (argc < 3)
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
			printf("You have uncommitted files. You can`t checkout\n");
			return 1;
		}
		if (chdir(originalCwd) != 0)
		{
			perror("Error restoring original directory");
			return 1;
		}

		int commit_ID = atoi(argv[2]);

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
					checkout_file(entry->d_name, find_file_last_change_before_commit(entry->d_name, commit_ID), filePath);
			}
		}
		closedir(dir);
		 
		 char *current_branch = (char *)malloc(50 * sizeof(char));
        current_branch = currentBranch();
        if (strcmp(current_branch, "master") == 0)
        {
			int last_commitID_master = extract_lastID_master();
			if (commit_ID == last_commitID_master) {
				makeHEADone_aftergoing_head();
			} else {
                    makeHEADzero_afterchkot_byID();
			}
		} else {
			int last_commitID_branch = extract_lastID_branch(current_branch);
			if (commit_ID == last_commitID_branch) {
				makeHEADone_aftergoing_head();
			} else {
                    makeHEADzero_afterchkot_byID();
			}
		}
		
		int new_currentID = atoi(argv[2]);
		change_current_ID(new_currentID);

		return 0;
	}
}

bool is_tracked(char *filepath)
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
	char trPath[PATH_MAX];
	snprintf(trPath, sizeof(trPath), "%s/.neogit/tracks", currentDir);
	FILE *file = fopen(trPath, "r");
	if (file == NULL)
		return false;
	char line[1024];
	while (fgets(line, sizeof(line), file) != NULL)
	{
		int length = strlen(line);

		// remove '\n'
		if (length > 0 && line[length - 1] == '\n')
		{
			line[length - 1] = '\0';
		}

		if (strcmp(line, filepath) == 0)
			return true;
	}
	fclose(file);

	return false;
}

int find_file_last_change_before_commit(char *filename, int commit_ID)
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
	char filPath[PATH_MAX];
	snprintf(filPath, sizeof(filPath), "%s/.neogit/all_files/", currentDir);
	char filepath_dir[1024];
	strcpy(filepath_dir, filPath);
	strcat(filepath_dir, filename);
	strcat(filepath_dir, "/");

	int max = -1;

	DIR *dir = opendir(filepath_dir);
	struct dirent *entry2;
	if (dir == NULL)
		return 1;

	while ((entry2 = readdir(dir)) != NULL)
	{
		if (entry2->d_type == DT_REG)
		{
			int tmp = atoi(entry2->d_name);
			if (tmp > max && tmp <= commit_ID)
			{
				max = tmp;
			}
		}
	}
	closedir(dir);

	return max;
}

int checkout_file(char *filename, int commit_ID, char *filepath)
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
	char filPath[PATH_MAX];
	snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);
	char src_file[1024];
	strcpy(src_file, filPath);
	strcat(src_file, filename);
	char tmp[10];
	sprintf(tmp, "/%d", commit_ID);
	strcat(src_file, tmp);

	FILE *read_file = fopen(src_file, "r");
	if (read_file == NULL)
		return 1;
	FILE *write_file = fopen(filepath, "w");
	if (write_file == NULL)
		return 1;

	char line[1024];

	while (fgets(line, sizeof(line), read_file) != NULL)
	{
		fprintf(write_file, "%s", line);
	}

	fclose(read_file);
	fclose(write_file);

	return 0;
}