#include "file_functions.h"

int run_commit(int argc, char *const argv[])
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
        char filPath[PATH_MAX];
        snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);
        char allfilesPath[PATH_MAX];
        snprintf(allfilesPath, sizeof(allfilesPath), "%s/.neogit/all_files/", currentDir);
        char brPath[PATH_MAX];
        snprintf(brPath, sizeof(brPath), "%s/.neogit/branches/", currentDir);

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

        fseek(file, 0, SEEK_END); // Move the file pointer to the end of the file
        long size = ftell(file);  // Get the position, which represents the file size
        rewind(file);

        if (size == 0)
        {
            perror("nothing to commit");
            return 1; // File is empty
        }

        int commit_ID = inc_last_commit_ID_total();
        if (commit_ID == -1)
            return 1;
        change_last_commit_ID(commit_ID);   // for master
        change_current_ID(commit_ID);    // total
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

            if (!check_file_directory_exists(fileName))
            {
                char dir_path[PATH_MAX];
                strcpy(dir_path, filPath);
                strcat(dir_path, fileName);
                if (mkdir(dir_path, 0755) != 0)
                    return 1;
            }
            if (!check_file_directory_exists_total(fileName))
            {
                char dir_path[PATH_MAX];
                strcpy(dir_path, allfilesPath);
                strcat(dir_path, fileName);
                if (mkdir(dir_path, 0755) != 0)
                    return 1;
            }
             if (!check_mas_directory_exists())
            {
                char dir_path[PATH_MAX];
                strcpy(dir_path, brPath);
                strcat(dir_path, "master");
                if (mkdir(dir_path, 0755) != 0)
                    return 1;
            }
            char line1[1024];
            sscanf(line, "%s", line1);
            long int modtime;
            sscanf(line, "%*s\t%ld", &modtime);
            commit_staged_file(commit_ID, line1, modtime);
            saveContent(commit_ID, line1, fileName);
            saveContent_for_branch(commit_ID, line1, fileName, "master");
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

void change_last_commit_ID(int new_id)
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
    }
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return;
    char tmpconfPath[PATH_MAX];
    snprintf(tmpconfPath, sizeof(tmpconfPath), "%s/.neogit/tmp_config", currentDir);
    FILE *tmp_file = fopen(tmpconfPath, "w");
    if (tmp_file == NULL)
        return;

    int last_commit_ID;
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "last_commit_ID", 14) == 0)
		{
			fprintf(tmp_file, "last_commit_ID: %d\n", new_id);
		}
		else
		{
			fprintf(tmp_file, "%s", line);
		}
    }
    fclose(file);
    fclose(tmp_file);

    remove(confPath);
    rename(tmpconfPath, confPath);
}
int inc_last_commit_ID_total()
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
    }
    char confPath[PATH_MAX];
    snprintf(confPath, sizeof(confPath), "%s/.neogit/config", currentDir);
    FILE *file = fopen(confPath, "r");
    if (file == NULL)
        return -1;
    char tmpconfPath[PATH_MAX];
    snprintf(tmpconfPath, sizeof(tmpconfPath), "%s/.neogit/tmp_config", currentDir);
    FILE *tmp_file = fopen(tmpconfPath, "w");
    if (tmp_file == NULL)
        return -1;

    int last_commit_ID_total;
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "last_id_total", 13) == 0)
        {
            sscanf(line, "last_id_total: %d\n", &last_commit_ID_total);
            last_commit_ID_total++;
            fprintf(tmp_file, "last_id_total: %d\n", last_commit_ID_total);
        }
        else
            fprintf(tmp_file, "%s", line);
    }
    fclose(file);
    fclose(tmp_file);

    remove(confPath);
    rename(tmpconfPath, confPath);
    return last_commit_ID_total;
}

bool check_file_directory_exists(char *filepath)
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
    }
    char filPath[PATH_MAX];
    snprintf(filPath, sizeof(filPath), "%s/.neogit/files", currentDir);
    DIR *dir = opendir(filPath);
    struct dirent *entry2;
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

bool check_file_directory_exists_total(char *filepath)
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
    }
    char allfilesPath[PATH_MAX];
    snprintf(allfilesPath, sizeof(allfilesPath), "%s/.neogit/all_files", currentDir);
    DIR *dir = opendir(allfilesPath);
    struct dirent *entry2;
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

int commit_staged_file(int commit_ID, char *filepath, time_t modification_time)
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
    }
    char comPath[PATH_MAX];
    snprintf(comPath, sizeof(comPath), "%s/.neogit/commits/", currentDir);
    FILE *write_file;
    char write_path[1024];
    strcpy(write_path, comPath);
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(write_path, tmp);

    write_file = fopen(write_path, "a+");
    if (write_file == NULL)
        return 1;


    fprintf(write_file, "%s\t%ld\n", filepath, modification_time);
    fclose(write_file);

    return 0;
}

int saveContent(int commit_ID, char *filepath, char *filename)
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
    }
    char filPath[PATH_MAX];
    snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);
    FILE *read_file, *write_file;
    char read_path[1024];
    strcpy(read_path, filepath);
    char write_path[1024];
    strcpy(write_path, filPath);
    strcat(write_path, filename);
    strcat(write_path, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(write_path, tmp);

    read_file = fopen(read_path, "r");
    if (read_file == NULL)
        return 1;

    write_file = fopen(write_path, "w");
    if (write_file == NULL)
        return 1;

    char buffer;
    buffer = fgetc(read_file);
    while (buffer != EOF)
    {
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
    }
    char trPath[PATH_MAX];
    snprintf(trPath, sizeof(trPath), "%s/.neogit/tracks", currentDir);

    FILE *file = fopen(trPath, "a");
    if (file == NULL)
        return 1;
    fprintf(file, "%s\n", filepath);
    return 0;
}

int saveContent_total(int commit_ID, char *filepath, char *filename)
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
    }
    char filPath[PATH_MAX];
    snprintf(filPath, sizeof(filPath), "%s/.neogit/all_files/", currentDir);
    FILE *read_file, *write_file;
    char read_path[1024];
    strcpy(read_path, filepath);
    char write_path[1024];
    strcpy(write_path, filPath);
    strcat(write_path, filename);
    strcat(write_path, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(write_path, tmp);

    read_file = fopen(read_path, "r");
    if (read_file == NULL)
        return 1;

    write_file = fopen(write_path, "w");
    if (write_file == NULL)
        return 1;

    char buffer;
    buffer = fgetc(read_file);
    while (buffer != EOF)
    {
        fputc(buffer, write_file);
        buffer = fgetc(read_file);
    }
    fclose(read_file);
    fclose(write_file);

    return 0;
}

int find_file_last_commit(char *filepath)
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
    }
    char filPath[PATH_MAX];
    snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);
    char filepath_dir[1024];
    strcpy(filepath_dir, filPath);
    strcat(filepath_dir, filepath);

    int max = -1;

    DIR *dir = opendir(filepath_dir);
    struct dirent *entry2;
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

int create_log_file(int commit_ID, char *message, char *time, char *branch, char *author)
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
    }
    char comPath[PATH_MAX];
    snprintf(comPath, sizeof(comPath), "%s/.neogit/commits/", currentDir);
    char tmpcomPath[PATH_MAX];
    snprintf(tmpcomPath, sizeof(tmpcomPath), "%s/.neogit/temp_log", currentDir);
    char stgPath[PATH_MAX];
    snprintf(stgPath, sizeof(stgPath), "%s/.neogit/staging", currentDir);

    char commit_filepath[1024];
    strcpy(commit_filepath, comPath);
    strcat(commit_filepath, "log");

    FILE *file = fopen(commit_filepath, "a+");
    if (file == NULL)
        return 1;

    // Read existing content
    char existing_content[4096];
    size_t existing_content_size = fread(existing_content, 1, sizeof(existing_content), file);
    fseek(file, 0, SEEK_SET);

    // Open the file again in write mode to overwrite existing content
    FILE *temp_file = fopen(tmpcomPath, "w");
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
    FILE *stgfile = fopen(stgPath, "r");
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
    rename(tmpcomPath, commit_filepath);

    return 0;
}