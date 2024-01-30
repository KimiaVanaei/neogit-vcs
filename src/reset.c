#include "file_functions.h"

int remove_from_staging(char *filename)
{
    const char *staging_file_path = ".neogit/staging";

    FILE *staging_file = fopen(staging_file_path, "r");
    if (staging_file == NULL)
    {
        perror("Error opening staging file");
        return 1;
    }

    FILE *temp_file = fopen(".neogit/staging_temp", "w");
    if (temp_file == NULL)
    {
        perror("Error creating temporary file");
        fclose(staging_file);
        return 1;
    }

    char line[1024];

    while (fgets(line, sizeof(line), staging_file) != NULL)
    {
        int length = strlen(line);

        if (length > 0 && line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }
        if (strstr(line, filename) == NULL)
        {
           fprintf(temp_file, "%s\n", line);
        }

    }

    fclose(staging_file);
    fclose(temp_file);

    if (rename(".neogit/staging_temp", staging_file_path) != 0)
    {
        perror("Error updating staging file");
        return 1;
    }
    return 0;
}