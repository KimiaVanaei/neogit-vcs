#include "file_functions.h"

// struct tagInfo {
//     char tagname[50];
//     int commitID;
//     char author[50];
//     char message[100];
//     char operationTime[20];
// };

void swap(struct tagInfo *a, struct tagInfo *b) {
    struct tagInfo temp;
    memcpy(&temp, a, sizeof(struct tagInfo));
    memcpy(a, b, sizeof(struct tagInfo));
    memcpy(b, &temp, sizeof(struct tagInfo));
}

void printTagNamesInOrder() {
    char line[100];
    struct tagInfo tags[1000];
    int numTags = 0;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return ;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return ;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return ;

        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return ;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return ;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
		return ;
    } else{
        printf("neogit has not initialized\n");
        return;
    }
	char tagsfilePath[PATH_MAX];
    snprintf(tagsfilePath, sizeof(tagsfilePath), "%s/.neogit/tags", currentDir);
    FILE *file = fopen(tagsfilePath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &tags[numTags].commitID) == 1)
        {
            fgets(line, sizeof(line), file);
            sscanf(line, "tagname: %[^\n]", tags[numTags].tagname);
            fgets(line, sizeof(line), file);
            sscanf(line, "author: %[^\n]", tags[numTags].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", tags[numTags].message);
            fgets(line, sizeof(line), file);
            sscanf(line, "Date: %[^\n]", tags[numTags].operationTime);
            numTags++;
        }
    }
    fclose(file);
    for (int i = 0; i < numTags - 1; i++) {
        for (int j = 0; j < numTags - i - 1; j++) {
            if (strcmp(tags[j].tagname, tags[j + 1].tagname) > 0) {
                swap(&tags[j], &tags[j + 1]);
            }
        }
    }
    for (int i = 0; i < numTags; i++) {
        printf("%s\n", tags[i].tagname);
    }
}

void print_target_tag(char *tagname)
{
    char line[100];
    struct tagInfo tags[1000];
    int numTags = 0;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return ;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return ;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return ;

        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return ;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return ;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
		return ;
    } else{
        printf("neogit has not initialized\n");
        return;
    }
	char tagsfilePath[PATH_MAX];
    snprintf(tagsfilePath, sizeof(tagsfilePath), "%s/.neogit/tags", currentDir);
    FILE *file = fopen(tagsfilePath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &tags[numTags].commitID) == 1)
        {
            fgets(line, sizeof(line), file);
            sscanf(line, "tagname: %[^\n]", tags[numTags].tagname);
            fgets(line, sizeof(line), file);
            sscanf(line, "author: %[^\n]", tags[numTags].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", tags[numTags].message);
            fgets(line, sizeof(line), file);
            sscanf(line, "Date: %[^\n]", tags[numTags].operationTime);
            numTags++;
        }
    }
    fclose(file);

    for (int i = 0; i < numTags; ++i)
    {
        if (strcmp(tagname, tags[i].tagname) == 0)
        {
            printf("Commit ID: %d\n", tags[i].commitID);
            printf("Tagname: %s\n", tags[i].tagname);
            printf("Author: %s\n", tags[i].author);
            printf("Message: %s\n", tags[i].message);
            printf("Date: %s\n", tags[i].operationTime);
        }
    }
}

int add_info_to_tags_file(int commit_ID, char *message, char *time, char *tagname, char *author)
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
    char tagsPath[PATH_MAX];
    snprintf(tagsPath, sizeof(tagsPath), "%s/.neogit/tags", currentDir);
    char tags_filepath[PATH_MAX];
    strcpy(tags_filepath, tagsPath);
    if (check_tag_exists(tagname) == true)
    {
        printf("This tagname already exists. Choose another name.\n");
        return 1;
    }
    FILE *file = fopen(tags_filepath, "a+");
    if (file == NULL)
        return 1;
    fprintf(file, "commit ID: %d\n", commit_ID);
    fprintf(file, "tagname: %s\n", tagname);
    fprintf(file, "author: %s\n", author);
    fprintf(file, "message: %s\n", message);
    fprintf(file, "Date: %s\n", time);
    fclose(file);
    return 0;
}

bool check_tag_exists(char *tagname)
{
    char line[100];
    struct tagInfo tags[1000];
    int numTags = 0;
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
            return 1 ;
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
    } else{
        printf("neogit has not initialized\n");
        return 1;
    }
	char tagsfilePath[PATH_MAX];
    snprintf(tagsfilePath, sizeof(tagsfilePath), "%s/.neogit/tags", currentDir);
    FILE *file = fopen(tagsfilePath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &tags[numTags].commitID) == 1)
        {
            fgets(line, sizeof(line), file);
            sscanf(line, "tagname: %[^\n]", tags[numTags].tagname);
            fgets(line, sizeof(line), file);
            sscanf(line, "author: %[^\n]", tags[numTags].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", tags[numTags].message);
            fgets(line, sizeof(line), file);
            sscanf(line, "Date: %[^\n]", tags[numTags].operationTime);
            numTags++;
        }
    }
    fclose(file);
    for (int i = 0; i < numTags; ++i)
    {
        if (strcmp(tagname, tags[i].tagname) == 0)
        {
            return true;
        }
    }
    return false;
}

int add_info_to_tags_file_tekrri(int commit_ID, char *message, char *time, char *tagname, char *author)
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
    char tagsPath[PATH_MAX];
    snprintf(tagsPath, sizeof(tagsPath), "%s/.neogit/tags", currentDir);
    char tags_filepath[PATH_MAX];
    strcpy(tags_filepath, tagsPath);
    if (check_tag_exists(tagname) == true)
    {
        overwrite_existing_tag(commit_ID, message, time, tagname, author);
        return 0;
    }
    FILE *file = fopen(tags_filepath, "a+");
    if (file == NULL)
        return 1;
    fprintf(file, "commit ID: %d\n", commit_ID);
    fprintf(file, "tagname: %s\n", tagname);
    fprintf(file, "author: %s\n", author);
    fprintf(file, "message: %s\n", message);
    fprintf(file, "Date: %s\n", time);
    fclose(file);
    return 0;
}

void overwrite_existing_tag(int commit_ID, char *message, char *time, char *tagname, char *author)
{
    char line[100];
    struct tagInfo tags[1000];
    int numTags = 0;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return ;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return ;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return ;

        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return ;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return ;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
		return ;
    } else{
        printf("neogit has not initialized\n");
        return;
    }
	char tagsfilePath[PATH_MAX];
    snprintf(tagsfilePath, sizeof(tagsfilePath), "%s/.neogit/tags", currentDir);
    FILE *file = fopen(tagsfilePath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &tags[numTags].commitID) == 1)
        {
            fgets(line, sizeof(line), file);
            sscanf(line, "tagname: %[^\n]", tags[numTags].tagname);
            fgets(line, sizeof(line), file);
            sscanf(line, "author: %[^\n]", tags[numTags].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", tags[numTags].message);
            fgets(line, sizeof(line), file);
            sscanf(line, "Date: %[^\n]", tags[numTags].operationTime);
            numTags++;
        }
    }
    fclose(file);
    int found = 0;
    for (int i = 0; i < numTags; ++i) {
        if (strcmp(tags[i].tagname, tagname) == 0) {
            found = 1;
            strncpy(tags[i].author, author, sizeof(tags[i].author) - 1);
            strncpy(tags[i].message, message, sizeof(tags[i].message) - 1);
            strncpy(tags[i].operationTime, time, sizeof(tags[i].operationTime) - 1);
            tags[i].commitID = commit_ID;
            break;
        }
    }
     file = fopen(tagsfilePath, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < numTags; ++i) {
        fprintf(file, "commit ID: %d\n", tags[i].commitID);
        fprintf(file, "tagname: %s\n", tags[i].tagname);
        fprintf(file, "author: %s\n", tags[i].author);
        fprintf(file, "message: %s\n", tags[i].message);
        fprintf(file, "Date: %s\n", tags[i].operationTime);
    }
    fclose(file);
}
