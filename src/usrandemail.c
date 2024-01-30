#include "file_functions.h"

void update_username(char *new_usr)
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
		FILE *file = fopen("/home/kimia/.neogitconfig/info.txt", "r");
		if (file == NULL)
			return;

		FILE *tmp_file = fopen("/home/kimia/.neogitconfig/tmpinfo.txt", "w");
		if (tmp_file == NULL)
		{
			fclose(file);
			return;
		}
		if (file == NULL)
			return;

		char line[1024];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strncmp(line, "username", 8) == 0)
			{
				fprintf(tmp_file, "username: %s\n", new_usr);
			}
			else
			{
				fprintf(tmp_file, "%s", line);
			}
		}
		fclose(file);
		fclose(tmp_file);
		remove("/home/kimia/.neogitconfig/info.txt");
		rename("/home/kimia/.neogitconfig/tmpinfo.txt", "/home/kimia/.neogitconfig/info.txt");
	}
}
void update_email(char *new_eml)
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
		FILE *file = fopen("/home/kimia/.neogitconfig/info.txt", "r");
		if (file == NULL)
			return;

		FILE *tmp_file = fopen("/home/kimia/.neogitconfig/tmpinfo.txt", "w");
		if (tmp_file == NULL)
		{
			fclose(file);
			return;
		}
		char line[1024];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strncmp(line, "email", 5) == 0)
			{
				fprintf(tmp_file, "email: %s\n", new_eml);
			}
			else
			{
				fprintf(tmp_file, "%s", line);
			}
		}
		fclose(file);
		fclose(tmp_file);
		remove("/home/kimia/.neogitconfig/info.txt");
		rename("/home/kimia/.neogitconfig/tmpinfo.txt", "/home/kimia/.neogitconfig/info.txt");
	}
}