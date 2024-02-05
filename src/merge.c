#include "file_functions.h"

int run_merge(int first_id, int second_id)
{

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return -1;
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
            return -1;
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
            return -1;
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return -1;
        }
    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return -1 ;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return -1;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return -1;
        int merge = 0;
        char commits_path[PATH_MAX];
        snprintf(commits_path, sizeof(commits_path), "%s/.neogit/commits/", currentDir);
        char firstfile_path[PATH_MAX];
        strcpy(firstfile_path,commits_path);
        char tmp1[10];
        sprintf(tmp1, "%d", first_id);
        strcat(firstfile_path, tmp1);
        char scndfile_path[PATH_MAX];
        strcpy(scndfile_path,commits_path);
        char tmp2[10];
        sprintf(tmp2, "%d", second_id);
        strcat(scndfile_path, tmp2);
       FILE *firstfile = fopen(firstfile_path, "r");
    FILE *scndfile = fopen(scndfile_path, "r");

    if (firstfile == NULL || scndfile == NULL) {
        printf("Error opening files.\n");
        return -1;
    }

    char address1[PATH_MAX];
    char address2[PATH_MAX];
    int foundMatch;

    while (fscanf(firstfile, "%s %*d", address1) == 1) {
    rewind(scndfile);
    foundMatch = 0;

    while (fscanf(scndfile, "%s %*d", address2) == 1) {
        if (strcmp(address1 ,address2) == 0){
            foundMatch = 1;
            run_diff_for_matched2(address1, first_id, second_id);
            break;
        } 
    }

    if (!foundMatch) {
       run_add_single(address1);
       merge = 1;
    }
    }

// Add this line to reset the second file position
rewind(scndfile);

while (fscanf(scndfile, "%s %*d", address2) == 1) {
    rewind(firstfile);
    foundMatch = 0;

    while (fscanf(firstfile, "%s %*d", address1) == 1) {
        if (strcmp(address1 ,address2) == 0){
        foundMatch = 1;
        break;
        } 
    }

    if (!foundMatch) {
        run_add_single(address2);
        merge = 1;
    }
    }
    fclose(firstfile);
    fclose(scndfile);
    return merge;
    }
    
}


int add_merged_branch(char *branch_name)
{
    if (check_branch_directory_exists(branch_name))
    {
        printf("This branch already exists\n");
        return 1;
    }
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

    int last_commit_ID = extract_lastID_total();

    FILE *file2 = fopen(confPath, "a");
    if (file2 == NULL ) return -1;
    fprintf(file2, "%s %d\n", branch_name, last_commit_ID);
    fclose(file2);

    char brPath[PATH_MAX];
    snprintf(brPath, sizeof(brPath), "%s/.neogit/branches/", currentDir);
    char dir_path[PATH_MAX];
    strcpy(dir_path, brPath);
    strcat(dir_path, branch_name);
    strcat(dir_path, "/");

    if (mkdir(dir_path, 0755) != 0)
        return 1;

}

