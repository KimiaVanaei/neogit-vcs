#include "file_functions.h"

void run_preF(char *path1)
{
    FILE *f1 = fopen(path1, "r");
    if (f1 == NULL) {
        printf(RED "%s does not exist!\n" RESET, path1);
        fclose(f1);
        return;
    }
    if (!isStaged(path1)) {
        printf(RED "%s is not staged!\n" RESET, path1);
        return;
    }
            char *fileNamee1 = strrchr(path1, '/');
            char fileName1[50];

            if (fileNamee1 != NULL)
            {
                fileNamee1++;
            }
            sscanf(fileNamee1, "%s", fileName1);
            char *ftype1 = strrchr(fileName1, '.');
            if (ftype1 != NULL)
            {
                ftype1++;
            }
            
        if (strcmp(ftype1, "txt") == 0)
            {
                printf(BLU "\"%s\" :\n" RESET, fileName1);
                if (isIDpresentinFILE("todo-check"))
                printf("\"%s\"...........................................................%s\n", CYN "todo-check" RESET, (todo_chk_txt(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("eof-blank-space"))
                printf("\"%s\"...........................................................%s\n", CYN "eof-blank-space" RESET, (extra_wtspc(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("balance-braces"))
                printf("\"%s\"...........................................................%s\n", CYN "balance-braces" RESET, (checkBraceBalance(path1) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("format-check"))
                printf("\"%s\"...........................................................%s\n", CYN "format-check" RESET, (chk_format(path1) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("file-size-check"))
                printf("\"%s\"...........................................................%s\n", CYN "file-size-check" RESET, (size_chk(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("character-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "character-limit" RESET, (char_lim(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("indentation-check"))
                printf("\"%s\"...........................................................%s\n", CYN "indentation-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("static-error-check"))
                printf("\"%s\"...........................................................%s\n", CYN "static-error-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("time-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "time-limit" RESET, YEL "SKIPPED" RESET);
            }
            else if (strcmp(ftype1, "c") == 0 || strcmp(ftype1, "cpp") == 0)
            {
                printf(BLU "\"%s\" :\n" RESET, fileName1);
                if (isIDpresentinFILE("todo-check"))
                printf("\"%s\"...........................................................%s\n", CYN "todo-check" RESET, (todo_chk_txt(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("eof-blank-space"))
                printf("\"%s\"...........................................................%s\n", CYN "eof-blank-space" RESET, (extra_wtspc(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("balance-braces"))
                printf("\"%s\"...........................................................%s\n", CYN "balance-braces" RESET, (checkBraceBalance(path1) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("format-check"))
                printf("\"%s\"...........................................................%s\n", CYN "format-check" RESET, (chk_format(path1) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("file-size-check"))
                printf("\"%s\"...........................................................%s\n", CYN "file-size-check" RESET, (size_chk(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("character-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "character-limit" RESET, (char_lim(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                // if (isIDpresentinFILE("indentation-check"))
                // printf("\"%s\"...........................................................%s\n", CYN "indentation-check" RESET, YEL "SKIPPED" RESET);
                // if (isIDpresentinFILE("static-error-check"))
                // printf("\"%s\"...........................................................%s\n", CYN "static-error-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("time-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "time-limit" RESET, YEL "SKIPPED" RESET);
            }
            else if (strcmp(ftype1, "mp3") == 0 || strcmp(ftype1, "mp4") == 0 || strcmp(ftype1, "wav") == 0)
            {
                printf(BLU "\"%s\" :\n" RESET, fileName1);
                if (isIDpresentinFILE("todo-check"))
                printf("\"%s\"...........................................................%s\n", CYN "todo-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("eof-blank-space"))
                printf("\"%s\"...........................................................%s\n", CYN "eof-blank-space" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("balance-braces"))
                printf("\"%s\"...........................................................%s\n", CYN "balance-braces" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("format-check"))
                printf("\"%s\"...........................................................%s\n", CYN "format-check" RESET, (chk_format(path1) == 1) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("file-size-check"))
                printf("\"%s\"...........................................................%s\n", CYN "file-size-check" RESET, (size_chk(path1) == 0) ? GRN "PASSED" RESET : RED "FAILED" RESET);
                if (isIDpresentinFILE("character-limit"))
                printf("\"%s\"...........................................................%s\n", CYN "character-limit" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("indentation-check"))
                printf("\"%s\"...........................................................%s\n", CYN "indentation-check" RESET, YEL "SKIPPED" RESET);
                if (isIDpresentinFILE("static-error-check"))
                printf("\"%s\"...........................................................%s\n", CYN "static-error-check" RESET, YEL "SKIPPED" RESET);
                // if (isIDpresentinFILE("time-limit"))
                // printf("\"%s\"...........................................................%s\n", CYN "time-limit" RESET, YEL "SKIPPED" RESET);
            }
        return;       
}