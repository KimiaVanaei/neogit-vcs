#include "file_functions.h"
int run_commit_on_branch(int argc, char *const argv[], char *branchname)
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return 1;

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
			return 1;
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
			return 1;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return 1;
		}

	} while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

	if (!foundNeogit)
	{
		printf("neogit has not been initialized\n");

		return 1;
	}
	else
	{
		if (chdir(tmp_cwd) != 0)
		{
			perror("Error changing directory");
			return 1;
		}
		strcpy(currentDir, tmp_cwd);
		if (chdir(cwd) != 0)
			return 1;
		char neoPath[PATH_MAX];
		snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
		// char filPath[PATH_MAX];
		// snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);

		if (argc < 4)
		{
			perror("You have not provided a commit message");
			return 1;
		}

		char message[1024];
		strcpy(message, argv[3]);
		if (strlen(message) > 72)
		{
			perror("commit message should not exceed 72 characters");
			return 1;
		}

		FILE *file = fopen(neoPath, "r");
		if (file == NULL)
		{
			return 1;
		}
		fseek(file, 0, SEEK_END);
		long size = ftell(file);
		rewind(file);

		if (size == 0)
		{
			printf("nothing to commit!\n");
			return 1; // File is empty
		}

		int commit_ID = inc_last_commit_ID_total();
		if (commit_ID == -1)
			return 1;
        change_current_ID(commit_ID);    // total
		change_last_commit_ID_ofbrn(branchname, commit_ID); // for branch
		char line[1024];

		while (fgets(line, sizeof(line), file) != NULL)
		{
			int length = strlen(line);

			if (length > 0 && line[length - 1] == '\n')
			{
				line[length - 1] = '\0';
			}

			char *fileNamee = strrchr(line, '/');
			char fileName[50];

			if (fileNamee != NULL)
			{
				fileNamee++;
			}
			sscanf(fileNamee, "%s", fileName);

			// if (!check_file_directory_exists(fileName))
			// {
			// 	char dir_path[PATH_MAX];
			// 	strcpy(dir_path, filPath);
			// 	strcat(dir_path, fileName);
			// 	if (mkdir(dir_path, 0755) != 0)
			// 		return 1;
			// }
			char line1[1024];
			sscanf(line, "%s", line1);
			long int modtime;
            sscanf(line, "%*s\t%ld", &modtime);
			commit_staged_file(commit_ID, line1, modtime);
			saveContent_for_branch(commit_ID, line1, fileName, branchname);
			saveContent_total(commit_ID, line1, fileName);
			track_file(line1); 
		}
		fclose(file);

		time_t t = time(NULL);
		struct tm *current_time = localtime(&t);
		char time_str[50];
		strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time);
		char *branch = (char *)malloc(50 * sizeof(char));
		char *author = (char *)malloc(50 * sizeof(char));
		branch = currentBranch();
		author = currentAuthor();
		create_log_file(commit_ID, message, time_str, branch, author);

		file = fopen(neoPath, "w");
		if (file == NULL)
			return 1;
		fclose(file);

		fprintf(stdout, "commit successfully with commit ID : %d\n", commit_ID);
		fprintf(stdout, "operation done in : %s\n", time_str);
		fprintf(stdout, "commit message : %s\n", message);
		return 0;
	}
}

void change_last_commit_ID_ofbrn(char *branchname, int new_id)
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
		char confPath[PATH_MAX];
		snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
		FILE *file = fopen(confPath, "r");
		if (file == NULL)
			return ;
		char tmpconfPath[PATH_MAX];
		snprintf(tmpconfPath, sizeof(tmpconfPath), "%s/.neogit/tmp_config", currentDir);
		FILE *tmp_file = fopen(tmpconfPath, "w");
		if (tmp_file == NULL)
			return ;
	
		char line[1024];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strncmp(line, branchname, strlen(branchname)) == 0)
			{
				fprintf(tmp_file, "%s %d\n",branchname, new_id);
			}
			else
				fprintf(tmp_file, "%s", line);
		}
		fclose(file);
		fclose(tmp_file);

		remove(confPath);
		rename(tmpconfPath, confPath);
	}
}

int saveContent_for_branch(int commit_ID, char* filepath, char *filename, char *branchname)
{
	char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    char originalCwd[1024];
    strcpy(originalCwd, cwd);

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
            return 1;
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
            return 1;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 1;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit)
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return 1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return 1;
    
    char brPath[PATH_MAX];
    snprintf(brPath, sizeof(brPath), "%s/.neogit/branches/", currentDir);
	strcat(brPath, branchname);
	strcat(brPath, "/");
	char tmp[10];
	sprintf(tmp, "%d", commit_ID);
	strcat(brPath, tmp);
	strcat(brPath, "/");
	if (mkdir(brPath, 0755) != 0) return 1;
	strcat(brPath, filename);

	char write_path[PATH_MAX];
	strcpy(write_path, brPath);
	char read_path[1024];
    strcpy(read_path, filepath);
	FILE *write_file = fopen(write_path, "w");
	if (write_file == NULL) {
		return 1;}
	FILE *read_file = fopen(read_path, "r");
    if (read_file == NULL) return 1;
	char buffer;
    buffer = fgetc(read_file);
    while(buffer != EOF) {
        fputc(buffer, write_file);
        buffer = fgetc(read_file);
    }
    fclose(read_file);
    fclose(write_file);

    return 0;
	}
}