#include "file_functions.h"

void update_localusername(char *new_usr)
{
	FILE *file = fopen(".neogit/local_config", "r");
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
	remove(".neogit/local_config");
	rename(".neogit/tmp_config", ".neogit/local_config");
}
void update_localemail(char *new_eml)
{
	FILE *file = fopen(".neogit/local_config", "r");
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
	remove(".neogit/local_config");
	rename(".neogit/tmp_config", ".neogit/local_config");
}