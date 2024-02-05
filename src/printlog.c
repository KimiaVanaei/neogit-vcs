#include "file_functions.h"

// Define a structure to store commit information
struct CommitInfo
{
    int commitID;
    char author[50];
    char branch[50];
    char message[100];
    int numFiles;
    char operationTime[20];
};

int printLogContent()
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
    } else{
        printf("neogit has not initialized\n");
        return 1;
    }
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    char line[200];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);

    return 0;
}

int printLogContentbyNum(int n)
{
    char line[100];
    struct CommitInfo commits[1000];
    int numCommits = 0;
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
    } else{
        printf("neogit has not initialized\n");
        return 1;
    }
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1)
        {
            fgets(line, sizeof(line), file); // Read the message line
            sscanf(line, "author: %[^\n]", commits[numCommits].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "branch: %[^\n]", commits[numCommits].branch);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file); // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file); // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }

    fclose(file);

    int endIdx = (numCommits > n) ? n : numCommits;

    for (int i = 0; i < endIdx; i++)
    {
        printf("Commit ID: %d\n", commits[i].commitID);
        printf("Author: %s\n", commits[i].author);
        printf("Branch: %s\n", commits[i].branch);
        printf("Message: %s\n", commits[i].message);
        printf("Number of files committed: %d\n", commits[i].numFiles);
        printf("Operation done in: %s\n", commits[i].operationTime);
        printf("---------------------------------------\n");
    }

    return 0;
}

void filterCommitsSince(char *date)
{
    char line[100];
    struct CommitInfo commits[1000];
    int numCommits = 0;
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
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1)
        {
            fgets(line, sizeof(line), file); // Read the message line
            sscanf(line, "author: %[^\n]", commits[numCommits].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "branch: %[^\n]", commits[numCommits].branch);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file); // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file); // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }

    fclose(file);
    struct tm tmDate;
    strptime(date, "%Y-%m-%d %H:%M:%S", &tmDate);
    time_t targetTime = mktime(&tmDate);

    for (int i = 0; i < numCommits; ++i)
    {
        struct tm tmCommit;
        strptime(commits[i].operationTime, "%Y-%m-%d %H:%M:%S", &tmCommit);
        time_t commitTime = mktime(&tmCommit);

        if (commitTime >= targetTime)
        {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Author: %s\n", commits[i].author);
            printf("Branch: %s\n", commits[i].branch);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}

void filterCommitsBefore(char *date)
{
    char line[100];
    struct CommitInfo commits[1000];
    int numCommits = 0;
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
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1)
        {
            fgets(line, sizeof(line), file); // Read the message line
            sscanf(line, "author: %[^\n]", commits[numCommits].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "branch: %[^\n]", commits[numCommits].branch);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file); // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file); // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }

    fclose(file);
    struct tm tmDate;
    strptime(date, "%Y-%m-%d %H:%M:%S", &tmDate);
    time_t targetTime = mktime(&tmDate);

    for (int i = 0; i < numCommits; ++i)
    {
        struct tm tmCommit;
        strptime(commits[i].operationTime, "%Y-%m-%d %H:%M:%S", &tmCommit);
        time_t commitTime = mktime(&tmCommit);

        if (commitTime <= targetTime)
        {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Author: %s\n", commits[i].author);
            printf("Branch: %s\n", commits[i].branch);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}
void printLogContentbyAuthor(char *target_author)
{
    char line[100];
    struct CommitInfo commits[1000];
    int numCommits = 0;
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
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1)
        {
            fgets(line, sizeof(line), file); // Read the message line
            sscanf(line, "author: %[^\n]", commits[numCommits].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "branch: %[^\n]", commits[numCommits].branch);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file); // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file); // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }
    fclose(file);

    for (int i = 0; i < numCommits; ++i)
    {
        if (strcmp(target_author, commits[i].author) == 0)
        {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Author: %s\n", commits[i].author);
            printf("Branch: %s\n", commits[i].branch);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}

void printLogContentbyBranch(char *target_branch)
{
    char line[100];
    struct CommitInfo commits[1000];
    int numCommits = 0;
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
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1)
        {
            fgets(line, sizeof(line), file); // Read the message line
            sscanf(line, "author: %[^\n]", commits[numCommits].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "branch: %[^\n]", commits[numCommits].branch);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file); // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file); // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }
    fclose(file);

    for (int i = 0; i < numCommits; ++i)
    {
        if (strcmp(target_branch, commits[i].branch) == 0)
        {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Author: %s\n", commits[i].author);
            printf("Branch: %s\n", commits[i].branch);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}

void printLogContentbyWords(char **target_words, int num_words)
{
    char line[100];
    struct CommitInfo commits[1000];
    int numCommits = 0;

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
	char logPath[PATH_MAX];
    snprintf(logPath, sizeof(logPath), "%s/.neogit/commits/log", currentDir);
    FILE *file = fopen(logPath, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1)
        {
            fgets(line, sizeof(line), file); // Read the message line
            sscanf(line, "author: %[^\n]", commits[numCommits].author);
            fgets(line, sizeof(line), file);
            sscanf(line, "branch: %[^\n]", commits[numCommits].branch);
            fgets(line, sizeof(line), file);
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file); // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file); // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }
    fclose(file);

    for (int i = 0; i < numCommits; ++i)
    {
        int found_any_word = 0;

        for (int j = 0; j < num_words; ++j)
        {
            char *ptr = commits[i].message;

            while ((ptr = strstr(ptr, target_words[j])) != NULL)
            {
                if ((ptr == commits[i].message || !isalpha(ptr[-1])) &&
                    (!isalpha(ptr[strlen(target_words[j])]) || ptr[strlen(target_words[j])] == '\0'))
                {
                    found_any_word = 1;
                    break;  // No need to check further if one word is found
                }
                ptr++;
            }

            if (!found_any_word && matchesWildcard(commits[i].message, target_words[j])) {
                found_any_word = 1;
                break;
            }

            if (found_any_word)
            {
                break;  // No need to check further if any word is found
            }
        }

        if (found_any_word)
        {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Author: %s\n", commits[i].author);
            printf("Branch: %s\n", commits[i].branch);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}

int matchesWildcard(const char *str, const char *pattern) {
    while (*str && *pattern) {
        if (*pattern == '*') {
            while (*pattern == '*') {
                pattern++;
            }
            while (*str && *str != *pattern) {
                str++;
            }
        } else if (*str != *pattern) {
            return 0;
        } else {
            str++;
            pattern++;
        }
    }

    while (*pattern == '*') {
        pattern++;
    }

    return !*pattern;
}
