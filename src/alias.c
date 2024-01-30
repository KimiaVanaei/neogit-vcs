#include "file_functions.h"

int add_alias(char *newalias, char *command)
{
    FILE *file = fopen(".neogit/global_aliases", "a");
    if (file == NULL)
        return 1;
    fprintf(file, "%s: %s\n", newalias, command);
    fclose(file);
    return 0;
}
int add_localalias(char *newalias, char *command)
{
    FILE *file = fopen(".neogit/local_aliases", "a");
    if (file == NULL)
        return 1;
    fprintf(file, "%s: %s\n", newalias, command);
    fclose(file);
    return 0;
}

char *get_alias_from_file(char *filename, char *command)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    char line[1024]; 
    char *alias = NULL;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *token = strtok(line, ":");
        if (token != NULL && strcmp(token, command) == 0)
        {
            char *alias_value = strtok(NULL, "\n");
            if (alias_value != NULL)
            {
                alias = strdup(alias_value);
                break;
            }
        }
    }

    fclose(file);

    return alias;
}

char * check_and_replace_alias(char *command)
{
    // Check global_aliases file
    char *global_alias = get_alias_from_file(".neogit/global_aliases", command);
    if (global_alias != NULL)
    {
        strcpy(command, global_alias);
        free(global_alias);
        return command;
    }

    // Check local_aliases file
    char *local_alias = get_alias_from_file(".neogit/local_aliases", command);
    if (local_alias != NULL)
    {
        strcpy(command, local_alias);
        free(local_alias);
    }
}
