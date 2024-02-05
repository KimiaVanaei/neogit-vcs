#include "file_functions.h"

void find_diff_between_two(int first_id, int second_id)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return ;
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
            return ;
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
            return ;
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return ;
        }
    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return ;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return ;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return ;

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
        return ;
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
                run_diff_for_matched(address1, first_id, second_id);
                break;
            } 
        }

        if (!foundMatch) {
            printf("%s\n", address1);
        }
    }

    rewind(firstfile);  // Reset the first file position

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
            printf("%s\n", address2);
        }
    }
    fclose(firstfile);
    fclose(scndfile);
    }
}


char *strTrim(char *str) {
    size_t len = strlen(str);
    size_t start = 0;
    while (start < len && isspace(str[start])) {
        start++;
    }
    size_t end = len - 1;
    while (end > start && isspace(str[end])) {
        end--;
    }
    size_t trimmedLen = (end >= start) ? (end - start + 1) : 0;
    char *trimmedStr = malloc(trimmedLen + 1);
    if (trimmedStr == NULL) {
        return NULL;
    }
    strncpy(trimmedStr, str + start, trimmedLen);
    trimmedStr[trimmedLen] = '\0';
    return trimmedStr;
}

char *strDuplicate(const char *str) {
    size_t len = strlen(str);
    char *duplicate = malloc(len + 1);
    if (duplicate == NULL) {
        return NULL;
    }
    strcpy(duplicate, str);
    return duplicate;
}

int compareLines(FILE *baseFile, FILE *changedFile, char *line1, char *line2, unsigned int *line1_cnt, unsigned int *line2_cnt) {
    // Skip empty lines
    while (line1[0] == '\0' || line1[0] == '\n') {
        if (!fgets(line1, STR_LINE_MAX, baseFile)) {
            break;
        }
        (*line1_cnt)++;
        normalizeLineEndings(line1);
    }

    while (line2[0] == '\0' || line2[0] == '\n') {
        if (!fgets(line2, STR_LINE_MAX, changedFile)) {
            break;
        }
        (*line2_cnt)++;
        normalizeLineEndings(line2);
    }

    char *trimmed_line1 = strTrim(line1);
    char *trimmed_line2 = strTrim(line2);

    int result = strcmp(trimmed_line1, trimmed_line2);

    free(trimmed_line1);
    free(trimmed_line2);

    return result;
}


Diff getDiff(const char *baseFilePath, const char *changedFilePath, int f1begin, int f1end, int f2begin, int f2end) {
    Diff diff = {NULL, NULL, 0, NULL, NULL, 0};

    FILE *baseFile = fopen(baseFilePath, "r");
    FILE *changedFile = fopen(changedFilePath, "r");

    if (!baseFile || !changedFile) {
        return diff;
    }

    char line1[STR_LINE_MAX], line2[STR_LINE_MAX];
    unsigned int line1_cnt = f1begin - 1, line2_cnt = f2begin - 1;

    fseek(baseFile, 0, SEEK_SET);
    fseek(changedFile, 0, SEEK_SET);

    while (fgets(line1, sizeof(line1), baseFile) && fgets(line2, sizeof(line2), changedFile)) {
        line1_cnt++;
        line2_cnt++;

        if ((line1_cnt < f1begin || line1_cnt > f1end) || (line2_cnt < f2begin || line2_cnt > f2end)) {
            continue;
        }

        if (compareLines(baseFile, changedFile, line1, line2, &line1_cnt, &line2_cnt) == 0) {
            continue;
        }

        diff.deletedLines = realloc(diff.deletedLines, (diff.deletedCount + 1) * sizeof(char *));
        diff.lineNumberDeleted = realloc(diff.lineNumberDeleted, (diff.deletedCount + 1) * sizeof(unsigned int));
        diff.deletedLines[diff.deletedCount] = strDuplicate(line1);
        diff.lineNumberDeleted[diff.deletedCount] = line1_cnt;
        diff.deletedCount++;

        diff.addedLines = realloc(diff.addedLines, (diff.addedCount + 1) * sizeof(char *));
        diff.lineNumberAdded = realloc(diff.lineNumberAdded, (diff.addedCount + 1) * sizeof(unsigned int));
        diff.addedLines[diff.addedCount] = strDuplicate(line2);
        diff.lineNumberAdded[diff.addedCount] = line2_cnt;
        diff.addedCount++;
    }

    fclose(baseFile);
    fclose(changedFile);

    return diff;
}

void printDifferences(const Diff *diff, const char *baseFilePath, const char *changedFilePath) {
    size_t maxDeletedLines = diff->deletedCount;
    size_t maxAddedLines = diff->addedCount;
    char *file1Namee = strrchr(baseFilePath, '/');
            char file1Name[50];

            if (file1Namee != NULL)
            {
                file1Namee++;
            }
            sscanf(file1Namee, "%s", file1Name);
        char *file2Namee = strrchr(changedFilePath, '/');
        char file2Name[50];
        if (file2Namee != NULL)
        {
            file2Namee++;
        }
        sscanf(file2Namee, "%s", file2Name);

    if (maxDeletedLines == 0 && maxAddedLines == 0) {
        return;
    }

    for (size_t i = 0; i < maxDeletedLines || i < maxAddedLines; ++i) {
        if (i < maxDeletedLines) {
            printf(BRMAG "<< << << << <<\n%s-%u\n%s\n" RESET, file1Name, (diff->lineNumberDeleted[i])+1, diff->deletedLines[i]);
        } 
        if (i < maxAddedLines) {
            printf(YEL "%s-%u\n%s\n>> >> >> >> >>\n" RESET, file2Name, (diff->lineNumberAdded[i])+1, diff->addedLines[i]);
        } 
        printf("\n");
    }
}
void printDifferences_merge(const Diff *diff, const char *baseFilePath, const char *changedFilePath, const char *filename) {
    size_t maxDeletedLines = diff->deletedCount;
    size_t maxAddedLines = diff->addedCount;
    if (maxDeletedLines == 0 && maxAddedLines == 0) {
        return;
    }
    for (size_t i = 0; i < maxDeletedLines || i < maxAddedLines; ++i) {
        if (i < maxDeletedLines) {
            printf(BRMAG "<< << << << << %s >> >> >> >> >>\n%s-%u\n%s\n" RESET, baseFilePath, filename, (diff->lineNumberDeleted[i])+1, diff->deletedLines[i]);
        } 
        if (i < maxAddedLines) {
            printf(YEL "<< << << << << %s >> >> >> >> >>\n%u\n%s\n" RESET, changedFilePath, (diff->lineNumberAdded[i])+1, diff->addedLines[i]);
        } 
        printf("\n");
    }
}
void normalizeLineEndings(char *line) {
    size_t len = strlen(line);
    if (len > 0 && ((line[len - 1] == '\n') ||  (line[len - 1] == '\r'))) {
        line[len - 1] = '\0';  // Remove the newline character
    }
}

int countLines(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return -1; 
    }
    int lineCount = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineCount++;
        }
    }
    fclose(file);
    return lineCount;
}

void run_diff_for_matched(char *commonPath, int first_id, int scnd_id)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return ;
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
            return ;
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
            return ;
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return ;
        }
    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return ;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return ;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return ;

        char allfilesPath[PATH_MAX];
        snprintf(allfilesPath, sizeof(allfilesPath), "%s/.neogit/all_files/", currentDir);
    char firstCommitPath[PATH_MAX];
    char scndCommitPath[PATH_MAX];
    char *fileNamee = strrchr(commonPath, '/');
    char fileName[50];
            if (fileNamee != NULL)
            {
                fileNamee++;
            }
    sscanf(fileNamee, "%s", fileName);
    strcpy(firstCommitPath, allfilesPath);
    strcpy(scndCommitPath, allfilesPath);
    strcat(firstCommitPath, fileName);
    strcat(scndCommitPath, fileName);
    strcat(firstCommitPath, "/");
    strcat(scndCommitPath, "/");
    char tmp1[10];
    sprintf(tmp1, "%d", first_id);
    char tmp2[10];
    sprintf(tmp2, "%d", scnd_id);
    strcat(firstCommitPath, tmp1);
    strcat(scndCommitPath, tmp2);
    int f1end = countLines(firstCommitPath);
    int f2end = countLines(scndCommitPath);
    Diff result = getDiff(firstCommitPath, scndCommitPath, 0, f1end, 0, f2end);
     printDifferences(&result, firstCommitPath, scndCommitPath);
        for (size_t i = 0; i < result.deletedCount; i++) {
            free(result.deletedLines[i]);
        }
        free(result.deletedLines);
        free(result.lineNumberDeleted);
        for (size_t i = 0; i < result.addedCount; i++) {
            free(result.addedLines[i]);
        }
        free(result.addedLines);
        free(result.lineNumberAdded);
    }
}

void run_diff_for_matched2(char *commonPath, int first_id, int scnd_id)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return ;
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
            return ;
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
            return ;
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return ;
        }
    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return ;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return ;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return ;

        char allfilesPath[PATH_MAX];
        snprintf(allfilesPath, sizeof(allfilesPath), "%s/.neogit/all_files/", currentDir);
    char firstCommitPath[PATH_MAX];
    char scndCommitPath[PATH_MAX];
    char *fileNamee = strrchr(commonPath, '/');
    char fileName[50];
            if (fileNamee != NULL)
            {
                fileNamee++;
            }
    sscanf(fileNamee, "%s", fileName);
    strcpy(firstCommitPath, allfilesPath);
    strcpy(scndCommitPath, allfilesPath);
    strcat(firstCommitPath, fileName);
    strcat(scndCommitPath, fileName);
    strcat(firstCommitPath, "/");
    strcat(scndCommitPath, "/");
    char tmp1[10];
    sprintf(tmp1, "%d", first_id);
    char tmp2[10];
    sprintf(tmp2, "%d", scnd_id);
    strcat(firstCommitPath, tmp1);
    strcat(scndCommitPath, tmp2);
    int f1end = countLines(firstCommitPath);
    int f2end = countLines(scndCommitPath);
    Diff result = getDiff(firstCommitPath, scndCommitPath, 0, f1end, 0, f2end);
     printDifferences_merge(&result, firstCommitPath, scndCommitPath, fileName);
        for (size_t i = 0; i < result.deletedCount; i++) {
            free(result.deletedLines[i]);
        }
        free(result.deletedLines);
        free(result.lineNumberDeleted);
        for (size_t i = 0; i < result.addedCount; i++) {
            free(result.addedLines[i]);
        }
        free(result.addedLines);
        free(result.lineNumberAdded);
    }
}