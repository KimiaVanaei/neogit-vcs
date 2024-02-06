#include "file_functions.h"

int run_pre()
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
        printf(RED "neogit repository has not been initialized\n" RESET);

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
        FILE *file = fopen(neoPath, "r");
        if (file == NULL) {
            return 1;
        }
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
            char *ftype = strrchr(fileName, '.');
            if (ftype != NULL)
            {
                ftype++;
            }
            char fpath[1024];
            sscanf(line, "%s", fpath); //extracted file path from staging file
            if (strcmp(ftype, "txt") == 0)
            {
                printf(BLU "\"%s\" :\n" RESET, fileName);
                if (isIDpresentinFILE("todo-check"))
                printf("\"%s\"...........................................................%s\n", CYN "todo-check" RESET, (todo_chk_txt(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("eof-blank-space"))
                printf("\"%s\"...........................................................%s\n", CYN "eof-blank-space" RESET, (extra_wtspc(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("balance-braces"))
                printf("\"%s\"...........................................................%s\n", CYN "balance-braces" RESET, (checkBraceBalance(fpath) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("format-check"))
                printf("\"%s\"...........................................................%s\n", CYN "format-check" RESET, (chk_format(fpath) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("file-size-check"))
                printf("\"%s\"...........................................................%s\n", CYN "file-size-check" RESET, (size_chk(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("character-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "character-limit" RESET, (char_lim(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("indentation-check"))
                printf("\"%s\"...........................................................%s\n", CYN "indentation-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("static-error-check"))
                printf("\"%s\"...........................................................%s\n", CYN "static-error-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("time-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "time-limit" RESET, YEL "SKIPPED" RESET);
            }
            else if (strcmp(ftype, "c") == 0 || strcmp(ftype, "cpp") == 0)
            {
                printf(BLU "\"%s\" :\n" RESET, fileName);
                if (isIDpresentinFILE("todo-check"))
                printf("\"%s\"...........................................................%s\n", CYN "todo-check" RESET, (todo_chk_txt(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("eof-blank-space"))
                printf("\"%s\"...........................................................%s\n", CYN "eof-blank-space" RESET, (extra_wtspc(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("balance-braces"))
                printf("\"%s\"...........................................................%s\n", CYN "balance-braces" RESET, (checkBraceBalance(fpath) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("format-check"))
                printf("\"%s\"...........................................................%s\n", CYN "format-check" RESET, (chk_format(fpath) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("file-size-check"))
                printf("\"%s\"...........................................................%s\n", CYN "file-size-check" RESET, (size_chk(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("character-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "character-limit" RESET, (char_lim(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                // if (isIDpresentinFILE("indentation-check"))
                // printf("\"%s\"...........................................................%s\n", CYN "indentation-check" RESET, YEL "SKIPPED" RESET);
                // if (isIDpresentinFILE("static-error-check"))
                // printf("\"%s\"...........................................................%s\n", CYN "static-error-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("time-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "time-limit" RESET, YEL "SKIPPED" RESET);
            }
            else if (strcmp(ftype, "mp3") == 0 || strcmp(ftype, "mp4") == 0 || strcmp(ftype, "wav") == 0)
            {
                printf(BLU "\"%s\" :\n" RESET, fileName);
                if (isIDpresentinFILE("todo-check"))
                printf("\"%s\"...........................................................%s\n", CYN "todo-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("eof-blank-space"))
                printf("\"%s\"...........................................................%s\n", CYN "eof-blank-space" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("balance-braces"))
                printf("\"%s\"...........................................................%s\n", CYN "balance-braces" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("format-check"))
                printf("\"%s\"...........................................................%s\n", CYN "format-check" RESET, (chk_format(fpath) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("file-size-check"))
                printf("\"%s\"...........................................................%s\n", CYN "file-size-check" RESET, (size_chk(fpath) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("character-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "character-limit" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("indentation-check"))
                printf("\"%s\"...........................................................%s\n", CYN "indentation-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("static-error-check"))
                printf("\"%s\"...........................................................%s\n", CYN "static-error-check" RESET, YEL "SKIPPED" RESET);
                // if (isIDpresentinFILE("time-limit"))
                // printf("\"%s\"...........................................................%s\n", CYN "time-limit" RESET, YEL "SKIPPED" RESET);
            }
        }
            

    }
return 0;  
}

bool isIDpresentinFILE(char *pre)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return false;
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
            return false;
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
            return false;

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return false;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit)
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return false;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return false;
    }
    char hookPath[PATH_MAX];
    snprintf(hookPath, sizeof(hookPath), "%s/.neogit/applied_hooks", currentDir);
    char line[1024];
    FILE *file = fopen(hookPath, "r");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }
        if (strcmp(line, pre) == 0)
        {
            return true;
        }
    }
    return false;
    fclose(file);
}