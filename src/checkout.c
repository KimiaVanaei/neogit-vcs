#include "file_functions.h"

int run_checkout(int argc, char *const argv[])
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
	else {
 
	if (argc < 3)
		return 1;

	int commit_ID = atoi(argv[2]);

	DIR *dir = opendir(".");
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_REG && is_tracked(entry->d_name))
		{
			checkout_file(entry->d_name, find_file_last_change_before_commit(entry->d_name, commit_ID));
		}
	}
	closedir(dir);

	return 0;
	} 
}

bool is_tracked(char *filepath)
{
	FILE *file = fopen(".neogit/tracks", "r");
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

int find_file_last_change_before_commit(char *filepath, int commit_ID)
{
	char filepath_dir[1024];
	strcpy(filepath_dir, ".neogit/files/");
	strcat(filepath_dir, filepath);

	int max = -1;

	DIR *dir = opendir(filepath_dir);
	struct dirent *entry;
	if (dir == NULL)
		return 1;

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{
			int tmp = atoi(entry->d_name);
			if (tmp > max && tmp <= commit_ID)
			{
				max = tmp;
			}
		}
	}
	closedir(dir);

	return max;
}

int checkout_file(char *filepath, int commit_ID)
{
	char src_file[1024];
	strcpy(src_file, ".neogit/files/");
	strcat(src_file, filepath);
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
