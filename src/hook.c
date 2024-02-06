#include "file_functions.h"

void printHooksList()
{
     char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return;
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
    char hookPath[PATH_MAX];
    snprintf(hookPath, sizeof(hookPath), "%s/.neogit/all_hooks", currentDir);
    FILE *file = fopen(hookPath, "r");
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);

}

void addToAppliedHooks(char *newHook)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return;
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
    char hookPath[PATH_MAX];
    snprintf(hookPath, sizeof(hookPath), "%s/.neogit/applied_hooks", currentDir);
    FILE *file = fopen(hookPath, "a");
    fprintf(file,"%s\n",newHook);
    fclose(file);
}

void print_appliedHooksList()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return;
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
    char hookPath[PATH_MAX];
    snprintf(hookPath, sizeof(hookPath), "%s/.neogit/applied_hooks", currentDir);
    FILE *file = fopen(hookPath, "r");
    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);
}

void removeAppliedHook(char *target)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return;
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
    char hookPath[PATH_MAX];
    snprintf(hookPath, sizeof(hookPath), "%s/.neogit/applied_hooks", currentDir);
    char tmpPath[PATH_MAX];
    snprintf(tmpPath, sizeof(tmpPath), "%s/.neogit/tmp_applied_hooks", currentDir);
    FILE *file = fopen(hookPath, "r");
    if (file == NULL) return;
    FILE *tmp = fopen(tmpPath, "w");
    if (tmp == NULL) return;
    char line[100];
     while (fgets(line, sizeof(line), file) != NULL)
    {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }
        if (strcmp(line, target) != 0)
        {
            fprintf(tmp, "%s\n", line);
        }
    }
    fclose(file);
    fclose(tmp);
     if (rename(tmpPath, hookPath) != 0) {
        perror("Error renaming files");
        return ;
    }
}

int extra_wtspc(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    char ch;
    int newlineCount = 0;
    int found = 0;
      fseek(file, -1, SEEK_END);
    // Count the number of consecutive newlines
    while ((ch = fgetc(file)) == '\n') {
        newlineCount++;
        fseek(file, -2, SEEK_CUR); // Move the pointer two positions back
    }
    fclose(file);
    if (newlineCount > 0) {
        found = 1;
    } 
    return found;
}

int size_chk(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int moreThanFive = 0;
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size > 5 * 1024 * 1024) {
        moreThanFive = 1;
    }
    fclose(file);
    return moreThanFive;
}

int char_lim(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int moreThanLimit = 0;
    int charCount = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        charCount++;
    }
    if (charCount > 20000) {
        moreThanLimit = 1;
    }
    fclose(file);
    return moreThanLimit;
}

int todo_chk(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int hasTODO = 0;
    char line[1024];
    int insideMultilineComment = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        // Look for "//" style comments
        char *commentStart = strstr(line, "//");
        if (commentStart != NULL) {
            commentStart += 2;
            if (strstr(commentStart, "TODO") != NULL) {
                hasTODO = 1;
                fclose(file);
                return hasTODO;
            }
        }

        // Look for "/* */" style comments
        char *startMultilineComment = strstr(line, "/*");
        char *endMultilineComment = strstr(line, "*/");

        if (startMultilineComment != NULL) {
            insideMultilineComment = 1;
        }

        if (insideMultilineComment) {
            // Check if the comment contains "TODO"
            if (strstr(line, "TODO") != NULL) {
                hasTODO = 1;
                fclose(file);
                return hasTODO;
            }
        }
        if (endMultilineComment != NULL) {
            insideMultilineComment = 0;
        }
    }
    fclose(file);
    return hasTODO;
}

int isWord(const char *str) {
    int len = strlen(str);

    if (len == 0) {
        return 0;  // Empty string is not a word
    }

    for (int i = 0; i < len; ++i) {
        if (!isalpha(str[i]) && i == len - 1) {
            return 1;  // Word with a single punctuation mark at the end
        } else if (!isalpha(str[i])) {
            return 0;  // Not a word character
        }
    }

    return 1;  // Word with no punctuation or multiple punctuations
}

int containsTodo(const char *line) {
    const char *targetWord = "TODO";
    int targetWordLength = strlen(targetWord);

    char *ptr = (char *)line;

    while ((ptr = strstr(ptr, targetWord)) != NULL) {
        if ((ptr == line || !isalpha(ptr[-1])) &&
            (!isalpha(ptr[targetWordLength]) || ptr[targetWordLength] == '\0')) {
            return 1;  // "TODO" found as a standalone word
        }
        ptr++;
    }

    return 0;  // "TODO" not found as a standalone word
}

int todo_chk_txt(char *path)
{    

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1; 
    }

    char line[1024];
    int foundTodo = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (containsTodo(line)) {
            foundTodo = 1;
            return foundTodo;
        }
    }

    fclose(file);

    return foundTodo;
}


int checkBraceBalance(char *path) {

    FILE *file = fopen(path, "r");
    char stack[1000]; 
    int top = -1;  // Stack top index

    int lineNumber = 1;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lineNumber++;  // Increment line number on encountering newline character
        } else if (ch == '(' || ch == '[' || ch == '{') {
            if (top < 999) {
                stack[++top] = ch;  // Push opening brace onto the stack
            } else {
                fclose(file);
                return 0;  // Stack overflow
            }
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (top >= 0) {
                char openBrace = stack[top--];  // Pop the last opening brace from the stack

                if ((ch == ')' && openBrace != '(') ||
                    (ch == ']' && openBrace != '[') ||
                    (ch == '}' && openBrace != '{')) {
                    fclose(file);
                    return 0;  // Unmatched closing brace
                }
            } else {
                fclose(file);
                return 0;  // Unmatched closing brace
            }
        }
    }

    // Check for unmatched opening braces
    if (top >= 0) {
        fclose(file);
        return 0;  // Unmatched opening brace
    }
     
    fclose(file);
    return 1;  // Braces are balanced
}

int chk_format(char *path)
{
            char *fileNamee = strrchr(path, '/');
            char fileName[50];
            int correct = 0;

            if (fileNamee != NULL)
            {
                fileNamee++;
            }
            sscanf(fileNamee, "%s", fileName);
            char *ftype = strrchr(fileName, '.');
            if (ftype != NULL)
            {
                ftype++;
            }
       if (strcmp(ftype, "txt") == 0 || strcmp(ftype, "c") == 0 || strcmp(ftype, "cpp") == 0 || strcmp(ftype, "mp3") == 0 || strcmp(ftype, "mp4") == 0 || strcmp(ftype, "wav") == 0) {
        correct = 1;
        return correct;
      }
      return correct;
}
