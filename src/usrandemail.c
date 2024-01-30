#include "file_functions.h"

void update_username(char *new_usr)
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
void update_email(char *new_eml)
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