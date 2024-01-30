#include "file_functions.h"

int run_commit(int argc, char *const argv[])
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return 1;

	char tmp_cwd[1024];
	bool exists = false;
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
				exists = true;
		}
		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return 1;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return 1;
		}

	} while (strcmp(tmp_cwd, "/") != 0);

	if (chdir(cwd) != 0)
		return 1;

	if (!exists)
	{
		perror("neogit has not been initialized");
		return -1;
	}
	else
	{
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

		FILE *file = fopen(".neogit/staging", "r");
		if (file == NULL)
			return 1;
		fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
		long size = ftell(file);  // Get the position, which represents the file size
		rewind(file);

		if (size == 0)
		{
			perror("nothing to commit");
			return 1; // File is empty
		}

		int commit_ID = inc_last_commit_ID();
		if (commit_ID == -1)
			return 1;
		char line[1024];

		while (fgets(line, sizeof(line), file) != NULL)
		{
			int length = strlen(line);

			if (length > 0 && line[length - 1] == '\n')
			{
				line[length - 1] = '\0';
			}

			char *fileName = strrchr(line, '/'); 

			if (fileName != NULL)
			{
				fileName++;
			}

			if (!check_file_directory_exists(fileName))
			{
				char dir_path[PATH_MAX];
				strcpy(dir_path, ".neogit/files/");
				strcat(dir_path, fileName);
				if (mkdir(dir_path, 0755) != 0)
					return 1;
			}
			commit_staged_file(commit_ID, line);
			saveContent(commit_ID, line, fileName);
			track_file(line);
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

		file = fopen(".neogit/staging", "w");
		if (file == NULL)
			return 1;
		fclose(file);

		fprintf(stdout, "commit successfully with commit ID : %d\n", commit_ID);
		fprintf(stdout, "operation done in : %s\n", time_str);
		fprintf(stdout, "commit message : %s\n", message);
		return 0;
	}
}

int inc_last_commit_ID()
{
	FILE *file = fopen(".neogit/config", "r");
	if (file == NULL)
		return -1;

	FILE *tmp_file = fopen(".neogit/tmp_config", "w");
	if (tmp_file == NULL)
		return -1;

	int last_commit_ID;
	char line[1024];
	while (fgets(line, sizeof(line), file) != NULL)
	{
		if (strncmp(line, "last_commit_ID", 14) == 0)
		{
			sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
			last_commit_ID++;
			fprintf(tmp_file, "last_commit_ID: %d\n", last_commit_ID);
		}
		else
			fprintf(tmp_file, "%s", line);
	}
	fclose(file);
	fclose(tmp_file);

	remove(".neogit/config");
	rename(".neogit/tmp_config", ".neogit/config");
	return last_commit_ID;
}

bool check_file_directory_exists(char *filepath)
{
	DIR *dir = opendir(".neogit/files");
	struct dirent *entry;
	if (dir == NULL)
	{
		perror("Error opening current directory");
		return 1;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR && strcmp(entry->d_name, filepath) == 0)
			return true;
	}
	closedir(dir);

	return false;
}

int commit_staged_file(int commit_ID, char *filepath)
{

	FILE *write_file;
	char write_path[1024];
	strcpy(write_path, ".neogit/commits/");
	char tmp[10];
	sprintf(tmp, "%d", commit_ID);
	strcat(write_path, tmp);

	write_file = fopen(write_path, "a+");
	if (write_file == NULL)
		return 1;

	struct stat file_info;

	if (stat(filepath, &file_info) != 0)
	{
		perror("Error getting file information");
		return 1;
	}
	time_t modification_time = file_info.st_mtime;

	fprintf(write_file, "%s\t%ld\n", filepath, modification_time);
	fclose(write_file);

	return 0;
}

int saveContent(int commit_ID, char* filepath, char *filename) {
    FILE *read_file, *write_file;
    char read_path[1024];
    strcpy(read_path, filepath);
    char write_path[1024];
    strcpy(write_path, ".neogit/files/");
    strcat(write_path, filename);
    strcat(write_path, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(write_path, tmp);

    read_file = fopen(read_path, "r");
    if (read_file == NULL) return 1;

    write_file = fopen(write_path, "w");
    if (write_file == NULL) return 1;

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

int track_file(char *filepath)
{
	if (is_tracked(filepath))
		return 0;

	FILE *file = fopen(".neogit/tracks", "a");
	if (file == NULL)
		return 1;
	fprintf(file, "%s\n", filepath);
	return 0;
}

int find_file_last_commit(char *filepath)
{
	char filepath_dir[1024];
	strcpy(filepath_dir, ".neogit/files/");
	strcat(filepath_dir, filepath);

	int max = -1;

	DIR *dir = opendir(filepath_dir);
	struct dirent *entry;
	if (dir == NULL)
		return 1;

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{
			int tmp = atoi(entry->d_name);
			max = max > tmp ? max : tmp;
		}
	}
	closedir(dir);

	return max;
}

int create_log_file2(int commit_ID, char *message, char *time)
{
	char commit_filepath[1024];
	strcpy(commit_filepath, ".neogit/commits/");
	strcat(commit_filepath, "log");

	FILE *file = fopen(commit_filepath, "a+");
	if (file == NULL)
		return 1;
	fprintf(file, "commit ID: %d\n", commit_ID);
	fprintf(file, "message: %s\n", message);
	int tedad = 0;
	char ch;
	FILE *stgfile = fopen(".neogit/staging", "r");
	if (stgfile == NULL)
		return 1;
	while ((ch = fgetc(stgfile)) != EOF)
	{
		if (ch == '\n')
		{
			tedad++;
		}
	}
	fprintf(file, "number of files committed: %d\n", tedad);
	fprintf(file, "operation done in : %s\n", time);
	fprintf(file, "---------------------------------------\n");
	fclose(stgfile);

	fclose(file);
	return 0;
}
int create_log_file(int commit_ID, char *message, char *time, char *branch, char *author)
{
	char commit_filepath[1024];
	strcpy(commit_filepath, ".neogit/commits/");
	strcat(commit_filepath, "log");

	FILE *file = fopen(commit_filepath, "a+");
	if (file == NULL)
		return 1;

	// Read existing content
	char existing_content[4096];
	size_t existing_content_size = fread(existing_content, 1, sizeof(existing_content), file);
	fseek(file, 0, SEEK_SET);

	// Open the file again in write mode to overwrite existing content
	FILE *temp_file = fopen(".neogit/temp_log", "w");
	if (temp_file == NULL)
	{
		fclose(file);
		return 1;
	}

	// Write new log entry at the beginning
	fprintf(temp_file, "commit ID: %d\n", commit_ID);
	fprintf(temp_file, "author: %s\n", author);
	fprintf(temp_file, "branch: %s\n", branch);
	fprintf(temp_file, "message: %s\n", message);

	int tedad = 0;
	char ch;
	FILE *stgfile = fopen(".neogit/staging", "r");
	if (stgfile == NULL)
	{
		fclose(file);
		fclose(temp_file);
		return 1;
	}
	while ((ch = fgetc(stgfile)) != EOF)
	{
		if (ch == '\n')
		{
			tedad++;
		}
	}
	fprintf(temp_file, "number of files committed: %d\n", tedad);
	fprintf(temp_file, "operation done in : %s\n", time);
	fprintf(temp_file, "---------------------------------------\n");
	fclose(stgfile);

	// Append existing content after the new log entry
	fwrite(existing_content, 1, existing_content_size, temp_file);

	// Close and rename the temporary file to the original log file
	fclose(file);
	fclose(temp_file);
	remove(commit_filepath);
	rename(".neogit/temp_log", commit_filepath);

	return 0;
}
