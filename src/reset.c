#include "file_functions.h"

int remove_from_staging(char *filename)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return 1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return 1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
		return 1;
    } else {
        printf("neogit has not been initialized\n");
        return 1;
    }
    char stgPath[PATH_MAX];
	 snprintf(stgPath, sizeof(stgPath), "%s/.neogit/staging", currentDir);
    const char *staging_file_path = stgPath;

    FILE *staging_file = fopen(staging_file_path, "r");
    if (staging_file == NULL)
    {
        perror("Error opening staging file");
        return 1;
    }
    char tmpstgPath[PATH_MAX];
	snprintf(tmpstgPath, sizeof(tmpstgPath), "%s/.neogit/staging_temp", currentDir);

    FILE *temp_file = fopen(tmpstgPath, "w");
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
        char line1[1024];
        sscanf(line, "%s", line1);
        if (strstr(line1, filename) == NULL)
        {
           fprintf(temp_file, "%s\n", line);
        }

    }

    fclose(staging_file);
    fclose(temp_file);

    if (rename(tmpstgPath, staging_file_path) != 0)
    {
        perror("Error updating staging file");
        return 1;
    }
    return 0;
}


int removeRecentStaged()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return 1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return 1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
		return 1;
    } else {
        printf("neogit has not been initialized\n");
        return 1;
    }
    char stgPath[PATH_MAX];
	 snprintf(stgPath, sizeof(stgPath), "%s/.neogit/staging", currentDir);
    const char *staging_file_path = stgPath;

    FILE *staging_file = fopen(staging_file_path, "r");
    if (staging_file == NULL)
    {
        perror("Error opening staging file");
        return 1;
    }
    char tmpstgPath[PATH_MAX];
	snprintf(tmpstgPath, sizeof(tmpstgPath), "%s/.neogit/staging_temp", currentDir);
    FILE *temp_file = fopen(tmpstgPath, "w");
    if (temp_file == NULL)
    {
        perror("Error creating temporary file");
        fclose(staging_file);
        return 1;
    }

    char currentLine[1024];
    char firstLine[1024];
    long int firstLineNumber = 0;

    // Read the first line
    if (fgets(firstLine, sizeof(firstLine), staging_file) != NULL) {
        sscanf(firstLine, "%*s %ld", &firstLineNumber);
    } else {
        fprintf(stderr, "Error reading the first line\n");
        fclose(staging_file);
        fclose(temp_file);
        return 1;
    }

    // Process the remaining lines
    while (fgets(currentLine, sizeof(currentLine), staging_file) != NULL) {
        long int currentLineNumber;
        sscanf(currentLine, "%*s %ld", &currentLineNumber);

        if (currentLineNumber != firstLineNumber) {
            fprintf(temp_file, "%s", currentLine);
        }
        // Otherwise, skip the line
    }

    fclose(staging_file);
    fclose(temp_file);

    if (rename(tmpstgPath, stgPath) != 0) {
        perror("Error renaming files");
        return 1;
    }
return 0;

}