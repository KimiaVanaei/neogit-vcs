#include "file_functions.h"

int isWordMatch(const char *line, const char *searchWord) {
    // Find the word in the line
    const char *found = strstr(line, searchWord);

    // Check if it is a complete word
     while (found != NULL) {
        // Check for word boundaries
        if ((found == line || isspace(*(found - 1)) || ispunct(*(found - 1))) &&
            (isspace(*(found + strlen(searchWord))) || ispunct(*(found + strlen(searchWord))) || *(found + strlen(searchWord)) == '\0')) {
            return 1; // Found as a complete word
        }
        found = strstr(found + 1, searchWord); // Check for more occurrences
    }
    return 0;
}

void searchWordInFile(char *filePath, char *searchWord) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        const char *found = strstr(line, searchWord);
        if (isWordMatch(line, searchWord)) {
            printf("%.*s\033[1;31m%s\033[0m%s", (int)(found - line), line, searchWord, found + strlen(searchWord));
        }
    }
    fclose(file);
}
void searchWordInFileandPrintnum(char *filePath, char *searchWord) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    int lineNumber = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        const char *found = strstr(line, searchWord);
        if (isWordMatch(line, searchWord)) {
            printf("Line %d: %.*s\033[1;31m%s\033[0m%s", lineNumber, (int)(found - line), line, searchWord, found + strlen(searchWord));
        }
        lineNumber++;
    }

    fclose(file);
}

void searchWordInFile_in_a_id(char *filename, char *targetWord,int commit_ID)
{
    char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return;

	char tmp_cwd[1024];
	char currentDir[1024];
	bool foundNeogit = false;
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
			{
				foundNeogit = true;
				break;
			}
		}

		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return;
		}

	} while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

	if (foundNeogit)
	{
		if (chdir(tmp_cwd) != 0)
		{
			perror("Error changing directory");
			return;
		}
		strcpy(currentDir, tmp_cwd);
		if (chdir(cwd) != 0)
			return;
	
	char filPath[PATH_MAX];
	snprintf(filPath, sizeof(filPath), "%s/.neogit/all_files/", currentDir);
	char filepath_dir[1024];
	strcpy(filepath_dir, filPath);
	strcat(filepath_dir, filename);
	strcat(filepath_dir, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(filepath_dir, tmp);
    FILE *file = fopen(filepath_dir, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        const char *found = strstr(line, targetWord);
        if (isWordMatch(line, targetWord)) {
            printf("%.*s\033[1;31m%s\033[0m%s", (int)(found - line), line, targetWord, found + strlen(targetWord));
        }
    }
    fclose(file);
    }
}
void searchWordInFile_in_a_id_andprintNum(char *filename, char *targetWord,int commit_ID)
{
    char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return;

	char tmp_cwd[1024];
	char currentDir[1024];
	bool foundNeogit = false;
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
			{
				foundNeogit = true;
				break;
			}
		}

		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return;
		}

	} while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

	if (foundNeogit)
	{
		if (chdir(tmp_cwd) != 0)
		{
			perror("Error changing directory");
			return;
		}
		strcpy(currentDir, tmp_cwd);
		if (chdir(cwd) != 0)
			return;
	
	char filPath[PATH_MAX];
	snprintf(filPath, sizeof(filPath), "%s/.neogit/all_files/", currentDir);
	char filepath_dir[1024];
	strcpy(filepath_dir, filPath);
	strcat(filepath_dir, filename);
	strcat(filepath_dir, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(filepath_dir, tmp);
    FILE *file = fopen(filepath_dir, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    int lineNumber = 1;

    while (fgets(line, sizeof(line), file) != NULL) {
        const char *found = strstr(line, targetWord);
        if (isWordMatch(line, targetWord)) {
            printf("Line %d: %.*s\033[1;31m%s\033[0m%s", lineNumber, (int)(found - line), line, targetWord, found + strlen(targetWord));
        }
        lineNumber++;
    }

    fclose(file);
    }
}