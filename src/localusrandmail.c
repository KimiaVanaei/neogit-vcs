#include "file_functions.h"

void update_localusername(char *new_usr)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return;

	char tmp_cwd[1024];
	bool exists = false;
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
				exists = true;
		}
		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return;
		}

	} while (strcmp(tmp_cwd, "/") != 0);

	if (chdir(cwd) != 0)
		return;

	if (!exists)
	{
		perror("neogit has not been initialized");
		return;
	}
	else
	{
		FILE *file = fopen(".neogit/config", "r");
		if (file == NULL)
			return;

		FILE *tmp_file = fopen(".neogit/tmp_config", "w");
		if (tmp_file == NULL)
		{
			fclose(file);
			return;
		}
		char line[1024];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strncmp(line, "local username", 14) == 0)
			{
				fprintf(tmp_file, "local username: %s\n", new_usr);
			}
			else
			{
				fprintf(tmp_file, "%s", line);
			}
		}
		fclose(file);
		fclose(tmp_file);
		remove(".neogit/config");
		rename(".neogit/tmp_config", ".neogit/config");
	}
}
void update_localemail(char *new_eml)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return;

	char tmp_cwd[1024];
	bool exists = false;
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
				exists = true;
		}
		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return;
		}

	} while (strcmp(tmp_cwd, "/") != 0);

	if (chdir(cwd) != 0)
		return;

	if (!exists)
	{
		perror("neogit has not been initialized");
		return;
	}
	else
	{
		FILE *file = fopen(".neogit/config", "r");
		if (file == NULL)
			return;

		FILE *tmp_file = fopen(".neogit/tmp_config", "w");
		if (tmp_file == NULL)
		{
			fclose(file);
			return;
		}
		char line[1024];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strncmp(line, "local email", 11) == 0)
			{
				fprintf(tmp_file, "local email: %s\n", new_eml);
			}
			else
			{
				fprintf(tmp_file, "%s", line);
			}
		}
		fclose(file);
		fclose(tmp_file);
		remove(".neogit/config");
		rename(".neogit/tmp_config", ".neogit/config");
	}
}