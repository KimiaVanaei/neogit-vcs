#include "file_functions.h"
int run_revert(int argc, char *const argv[])
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

		// if (argc < 3)
		// 	return 1;
		// char neoPath[PATH_MAX];
		// snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
		// FILE *stgFile = fopen(neoPath, "r");
		// if (stgFile == NULL)
		// {
		// 	return 1;
		// }

		// fseek(stgFile, 0, SEEK_END);
		// long size = ftell(stgFile);	 
		// rewind(stgFile);

		// if (size != 0)
		// {
		// 	printf("You have uncommitted files. You can`t checkout\n");
		// 	return 1;
		// }
		if (chdir(originalCwd) != 0)
		{
			perror("Error restoring original directory");
			return 1;
		}

		int commit_ID = atoi(argv[3]);

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
		// int new_currentID = atoi(argv[2]);
		// change_current_ID(new_currentID);
		return 0;
	}
}

int run_revert_for_last_id(int argc, char *const argv[])
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

		// if (argc < 3)
		// 	return 1;
		// char neoPath[PATH_MAX];
		// snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
		// FILE *stgFile = fopen(neoPath, "r");
		// if (stgFile == NULL)
		// {
		// 	return 1;
		// }

		// fseek(stgFile, 0, SEEK_END);
		// long size = ftell(stgFile);	 
		// rewind(stgFile);

		// if (size != 0)
		// {
		// 	printf("You have uncommitted files. You can`t checkout\n");
		// 	return 1;
		// }
		if (chdir(originalCwd) != 0)
		{
			perror("Error restoring original directory");
			return 1;
		}
		int commit_ID;
        
		char *current_branch = (char *)malloc(50 * sizeof(char));
        current_branch = currentBranch();
        if (strcmp(current_branch, "master") == 0) {
		    commit_ID = extract_lastID_master();
		}else {
			commit_ID = extract_lastID_branch(current_branch);
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
					checkout_file(entry->d_name, find_file_last_change_before_commit(entry->d_name, commit_ID), filePath);
			}
		}
		closedir(dir);
		// int new_currentID = atoi(argv[2]);
		// change_current_ID(new_currentID);
		return 0;
	}
}

int run_revertm_HEAD(char *branchname, int N)
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
		// char neoPath[PATH_MAX];
		// snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
		// FILE *stgFile = fopen(neoPath, "r");
		// if (stgFile == NULL) return 1;
		// fseek(stgFile, 0, SEEK_END);
		// long size = ftell(stgFile);	 
		// rewind(stgFile);
		// if (size != 0)
		// {
		// 	printf("You have uncommitted files. You can`t checkout\n");
		// 	return 1;
		// }
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
					checkout_file_to_id_branch2(entry->d_name, filePath, branchname, N);
			}
		}
		closedir(dir);
		return 0;
    }
}


int checkout_file_to_id_branch2(char *filename, char *filepath, char *branchname, int N)
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
	char target_path[PATH_MAX];
	snprintf(filPath, sizeof(filPath), "%s/.neogit/branches/", currentDir);
    strcat(filPath, branchname);
    strcat(filPath, "/");                                                       
	char branchDir[PATH_MAX];
	strcpy(branchDir, filPath);
	DIR *dir2 = opendir(branchDir);
	
	int count = 0;
	int tedad_dirc = 0;
	struct dirent* entry2;
	 while ((entry2 = readdir(dir2)) != NULL) {
        if (entry2->d_type == DT_DIR && strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0) {
            tedad_dirc++;
        }
     }
	 rewinddir(dir2);
	 int n = tedad_dirc - N;
	while ((entry2 = readdir(dir2)) != NULL) {
        if (entry2->d_type == DT_DIR && strcmp(entry2->d_name , ".") != 0 && strcmp(entry2->d_name , "..") != 0) {
            count++;
            if (count == n) {
				strcpy(target_path, branchDir);
				strcat(target_path, entry2->d_name);
                strcat(target_path, "/");
                break;
            }
        }
    }
    closedir(dir2);

    char src_file[1024];
    strcpy(src_file, target_path);
    strcat(src_file, filename);
	FILE *read_file = fopen(src_file, "r");
	if (read_file == NULL) return 1;
	FILE *write_file = fopen(filepath, "w");
	if (write_file == NULL) return 1;
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