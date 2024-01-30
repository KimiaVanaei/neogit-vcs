#include "file_functions.h"

void update_localusername(char *new_usr)
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
void update_localemail(char *new_eml)
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