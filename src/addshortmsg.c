#include "file_functions.h"

int add_shrtcut_for_msg(char *newshortcut, char *msg)
{
    FILE *file = fopen(".neogit/shortcuts_for_msgs", "a");
    if (file == NULL)
        return 1;
    fprintf(file, "%s: %s\n", newshortcut, msg);
    fclose(file);
    return 0;
}

void update_msg(char *new_msg, char *shrtcut)
{
	FILE *file = fopen(".neogit/shortcuts_for_msgs", "r");
	if (file == NULL)
		return;

	FILE *tmp_file = fopen(".neogit/tmp_shortcuts_for_msgs", "w");
	if (tmp_file == NULL)
	{
		fclose(file);
		return;
	}
	char line[1024];
	while (fgets(line, sizeof(line), file) != NULL)
	{
		if (strncmp(line, shrtcut, strlen(shrtcut)) == 0)
		{
			fprintf(tmp_file, "%s: %s\n", shrtcut, new_msg);
		}
		else
		{
			fprintf(tmp_file, "%s", line);
		}
	}
	fclose(file);
	fclose(tmp_file);
	remove(".neogit/shortcuts_for_msgs");
	rename(".neogit/tmp_shortcuts_for_msgs", ".neogit/shortcuts_for_msgs");
}

void delete_msg(char *shrtcut)
{
    FILE *file = fopen(".neogit/shortcuts_for_msgs", "r");
	if (file == NULL)
		return;

	FILE *tmp_file = fopen(".neogit/tmp_shortcuts_for_msgs", "w");
	if (tmp_file == NULL)
	{
		fclose(file);
		return;
	}
    char line[1024];
	while (fgets(line, sizeof(line), file) != NULL)
	{
		if (strncmp(line, shrtcut, strlen(shrtcut)) == 0)
		{
			
		}
		else
		{
			fprintf(tmp_file, "%s", line);
		}
	}
	fclose(file);
	fclose(tmp_file);
	remove(".neogit/shortcuts_for_msgs");
	rename(".neogit/tmp_shortcuts_for_msgs", ".neogit/shortcuts_for_msgs");

}