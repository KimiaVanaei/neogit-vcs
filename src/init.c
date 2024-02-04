#include "file_functions.h"

int create_configs(char *username, char *email)
{
	FILE *file = fopen(".neogit/config", "w");
	if (file == NULL)
		return 1;

	fprintf(file, "local username: %s\n", username);
	fprintf(file, "local email: %s\n", email);
	fprintf(file, "last_commit_ID: %d\n", 0);
	fprintf(file, "HEAD(1 means u are in HEAD) %d\n", 1);
	fprintf(file, "branch: %s\n", "master");
	fprintf(file, "last_id_total: %d\n", 0);
	fprintf(file, "current_commit_ID: %d\n", 0);
	fclose(file);

	if (mkdir(".neogit/commits", 0755) != 0)
		return 1;

	if (mkdir(".neogit/files", 0755) != 0)
		return 1;
	if (mkdir(".neogit/all_files", 0755) != 0)
		return 1;
	if (mkdir(".neogit/branches", 0755) != 0)
		return 1;

	file = fopen(".neogit/staging", "w");
	fclose(file);

	file = fopen(".neogit/tracks", "w");
	fclose(file);

	FILE *file3 = fopen(".neogit/local_aliases", "w");
	if (file3 == NULL)
		return 1;
	fclose(file3);

	FILE *file5 = fopen(".neogit/shortcuts_for_msgs", "w");
	if (file5 == NULL)
		return 1;
	fclose(file5);

	FILE *file6 = fopen(".neogit/tags", "w");
	if (file6 == NULL)
		return 1;
	fclose(file6);

	return 0;
}

int run_init()
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

		// update current working directory
		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return 1;

		// change cwd to parent
		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return 1;
		}

	} while (strcmp(tmp_cwd, "/") != 0);

	// return to the initial cwd
	if (chdir(cwd) != 0)
		return 1;

	if (!exists)
	{
		if (mkdir(".neogit", 0755) != 0)
			return 1;
		return create_configs("_", "_");
	}
	else
	{
		perror("neogit repository has already initialized");
	}
	return 0;
}