#include "file_functions.h"

int main(int argc, char *argv[])
{
    
    if ((argc < 2))
    {
        printf(RED "INVALID COMMAND!\n" RESET);
        return 1;
    }
 
    if ((argc == 2) && strcmp(argv[1], "init") == 0)
    {
        return run_init();
    }
    else if ((argc == 5) && (strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "--global") == 0) && (strcmp(argv[3], "user.name") == 0))
    {
        char *new_username = argv[4];
        update_username(new_username);
    }
    else if ((argc == 5) && (strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "--global") == 0) && (strcmp(argv[3], "user.email") == 0))
    {
        char *new_email = argv[4];
        update_email(new_email);
    }
    else if ((argc == 4) && (strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "user.name") == 0))
    {
        char *new_username = argv[3];
        update_localusername(new_username);
    }
    else if ((argc == 4) && (strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "user.email") == 0))
    {
        char *new_email = argv[3];
        update_localemail(new_email);
    }
    else if ((argc == 5) && (strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "--global") == 0) && (strncmp(argv[3], "alias.", 6) == 0))
    {
        char *dotPosition = strchr(argv[3], '.');
        char *newalias = dotPosition + 1;
        add_alias(newalias, argv[4]);
    }
    else if ((argc == 4) && (strcmp(argv[1], "config") == 0) && (strncmp(argv[2], "alias.", 6) == 0))
    {
        char *dotPosition = strchr(argv[2], '.');
        char *newalias = dotPosition + 1;
        add_localalias(newalias, argv[3]);
    }
    else if ((argc == 2) && (strcmp(argv[1], "add") == 0))
    {
        printf(RED "Please specify a file!\n" RESET);
    }
    // else if ((argc == 3) && (strcmp(argv[1], "add") == 0) && (strcmp(argv[2], "-n") != 0) && (strcmp(argv[2], "-f") != 0))
    // {
    //     if (!containsWildcard(argv[2]))
    //     return run_add(argc, argv);                     ///<<<<<<<<<< UNCOMMENT IF FACED BUGS >>>>>>>>>>>>>///
    // }
    else if ((argc >= 3) && (strcmp(argv[1], "add") == 0) && (strcmp(argv[2], "-f") == 0))
    {
        for (int i = 3; i < argc; i++)
        {
            char filePath[PATH_MAX];
            if (realpath(argv[i], filePath) == NULL)
            {
                printf("%s does not exist\n", argv[i]);
                exit(EXIT_FAILURE);
            }

            if (run_add_single(filePath) != 0)
                fprintf(stderr, "Error processing: %s\n", argv[i]);
        }
    }
      if ((argc == 3) && (strcmp(argv[1], "add") == 0))
    {
        // if (containsWildcard(argv[2])) {
            glob_t glob_result;
            if (glob(argv[2], 0, NULL, &glob_result) != 0)
            {
                perror("Error expanding wildcard");
                return -1;
            }
            for (size_t i = 0; i < glob_result.gl_pathc; ++i)
            {
                char filePath[PATH_MAX];
                if (realpath(glob_result.gl_pathv[i], filePath) == NULL)
                {
                    printf("%s does not exist\n", glob_result.gl_pathv[i]);
                    exit(EXIT_FAILURE);
                }

                if (run_add_single(filePath) != 0)
                    fprintf(stderr, "Error processing: %s\n", glob_result.gl_pathv[i]);
            }
            globfree(&glob_result);
        // }
    }
    else if ((argc >= 2) && strcmp(argv[1], "commit") == 0 && (strcmp(argv[2], "-s") != 0))
    {
        char *current_branch = (char *)malloc(50 * sizeof(char));
        current_branch = currentBranch();
        int HEAD = HEADorNOT();
        if (HEAD == 0)
        {
            printf(RED "You can only commit on HEAD. Checkout to HEAD to commit\n" RESET);
            return 1;
        }
        else
        {
            if (strcmp(current_branch, "master") == 0)
            {
                return run_commit(argc, argv);
            }
            else{
                return run_commit_on_branch(argc, argv, current_branch);
            }
        }
    }
     //neogit checkout HEAD-n
    else if ((argc == 3) && (strcmp(argv[1], "checkout") == 0) && (strncmp(argv[2], "HEAD-", 5) == 0))
    {
        char* endptr;
        long n_value = strtol(argv[2] + 5, &endptr, 10);
        char *current_branch = (char *)malloc(50 * sizeof(char));
        current_branch = currentBranch();
        makeHEADzero_afterchkot_byID();
        return checkout_to_id_branch(current_branch, n_value );
    }
    else if ((argc == 3) && strcmp(argv[1], "checkout") == 0 && strcmp(argv[2], "HEAD") != 0)
    {
            return run_checkout(argc, argv);
    }
    else if ((argc >= 2) && strcmp(argv[1], "checkoutbr") == 0)
    {
        char *branchname = argv[2];
        change_branch_in_configs(branchname);
        if (strcmp(branchname, "master") == 0)
        {
            int last_commitID_master = extract_lastID_master();
            change_current_ID(last_commitID_master);
            return run_checkout_master(argc, argv);
        }else {
            int last_commitID_branch = extract_lastID_branch(branchname);
            change_current_ID(last_commitID_branch);
             return run_checkouttobranch(branchname);
        }
    }
    else if ((argc == 3) && strcmp(argv[1], "checkout") == 0 && strcmp(argv[2], "HEAD") == 0)
    {
        char *current_branch = (char *)malloc(50 * sizeof(char));
        current_branch = currentBranch();
        if (strcmp(current_branch, "master") == 0)
        {   
            int last_commitID_master = extract_lastID_master();
            change_current_ID(last_commitID_master);
             makeHEADone_aftergoing_head();
            return run_checkoutHEAD(argc, argv);
        }
        else
        {
            int last_commitID_branch = extract_lastID_branch(current_branch);
            change_current_ID(last_commitID_branch);
             makeHEADone_aftergoing_head();
            return run_checkoutHEAD_forbr(current_branch);
        }
    }
    // else if ((argc == 3) && strcmp(argv[1], "reset") == 0 && (strcmp(argv[2], "-f") != 0) && (strcmp(argv[2], "-undo") != 0))
    // {
    //     if (!containsWildcard(argv[2])) {
    //         char *target = argv[2];
    //         return remove_from_staging(target);
    //     }                                                   ///<<<<<<<<<< UNCOMMENT IF FACED BUGS >>>>>>>>>>>>>///
    // }
    else if ((argc >= 3) && (strcmp(argv[1], "reset") == 0) && (strcmp(argv[2], "-f") == 0))
    {
        for (int i = 3; i < argc; i++)
        {
            char filePath[PATH_MAX];
            if (realpath(argv[i], filePath) == NULL)
            {
                printf("%s does not exist\n", argv[i]);
                exit(EXIT_FAILURE);
            }
            if (remove_from_staging(argv[i]) != 0)
                fprintf(stderr, "Error processing: %s\n", argv[i]);
        }
    }
    if ((argc == 3) && (strcmp(argv[1], "reset") == 0) && (strcmp(argv[2], "-undo") != 0))
    {
        // if (containsWildcard(argv[2])) {
            glob_t glob_result;
            if (glob(argv[2], 0, NULL, &glob_result) != 0)
            {
                perror("Error expanding wildcard");
                return -1;
            }
            for (size_t i = 0; i < glob_result.gl_pathc; ++i)
            {
                char filePath[PATH_MAX];
                if (realpath(glob_result.gl_pathv[i], filePath) == NULL)
                {
                    printf("%s does not exist\n", glob_result.gl_pathv[i]);
                    exit(EXIT_FAILURE);
                }
                if (remove_from_staging(glob_result.gl_pathv[i]) != 0)
                fprintf(stderr, "Error processing: %s\n", argv[i]);
            }
            globfree(&glob_result);
        // }
    }
    else if ((argc == 3) && (strcmp(argv[1], "reset") == 0) && (strcmp(argv[2], "-undo") == 0))
    {
        return removeRecentStaged();
    }
    else if ((argc == 4) && (strcmp(argv[1], "add") == 0) && (strcmp(argv[2], "-n") == 0))
    {
        int depth = atoi(argv[3]);
        printf("%d\n", depth);
        add_depth(".", depth - 1);
    }
    // neogit set -m ”shortcut message” -s shortcut-name
    else if ((argc == 6) && (strcmp(argv[1], "set") == 0) && (strcmp(argv[2], "-m") == 0) && (strcmp(argv[4], "-s") == 0))
    {
        add_shrtcut_for_msg(argv[5], argv[3]);
    }
    else if ((argc == 4) && (strcmp(argv[1], "commit") == 0) && (strcmp(argv[2], "-s") == 0))
    {
        FILE *file = fopen(".neogit/shortcuts_for_msgs", "r");
        if (file == NULL)
            return -1;
        int flag = 0;

        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            char *token = strtok(line, ": ");
            if (token != NULL && strcmp(token, argv[3]) == 0)
            {
                flag = 1;
                char *savedmsg = strtok(NULL, "\n");
                if (savedmsg != NULL)
                {
                    strcpy(argv[3], savedmsg);

                    break;
                }
            }
        }
        if (!flag)
        {
            perror("shortcut does not exist");
            return -1;
        }

        fclose(file);
        return run_commit(argc, argv);
    }
    // neogit replace -m ”new shortcut message” -s shortcut-name
    else if ((argc == 6) && (strcmp(argv[1], "replace") == 0) && (strcmp(argv[2], "-m") == 0) && (strcmp(argv[4], "-s") == 0))
    {
        FILE *file = fopen(".neogit/shortcuts_for_msgs", "r");
        if (file == NULL)
            return -1;
        int flag = 0;
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            char *token = strtok(line, ": ");
            if (token != NULL && strcmp(token, argv[5]) == 0)
            {
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            perror("shortcut does not exist");
            return -1;
        }
        fclose(file);

        update_msg(argv[3], argv[5]);
    }
    // neogit remove -s shortcut-name
    else if ((argc == 4) && (strcmp(argv[1], "remove") == 0) && (strcmp(argv[2], "-s") == 0))
    {
        FILE *file = fopen(".neogit/shortcuts_for_msgs", "r");
        if (file == NULL)
            return -1;
        int flag = 0;
        char line[1024];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            char *token = strtok(line, ": ");
            if (token != NULL && strcmp(token, argv[3]) == 0)
            {
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            perror("shortcut does not exist");
            return 1;
        }
        fclose(file);

        delete_msg(argv[3]);
    }
    else if ((argc == 2) && (strcmp(argv[1], "log") == 0))
    {
        return printLogContent();
    }
    else if ((argc == 3) && (strcmp(argv[1], "log") == 0))
    {
        int n = atoi(argv[2]);
        return printLogContentbyNum(n);
    }
    // neogit log-since <date>
    else if ((argc == 4) && (strcmp(argv[1], "log") == 0) && (strcmp(argv[2], "-branch") != 0) && (strcmp(argv[2], "-author") != 0) && (strcmp(argv[2], "-search") != 0))
    {
        if (strcmp(argv[2], "-since") == 0)
        {
            filterCommitsSince(argv[3]);
        }
        else if (strcmp(argv[2], "-before") == 0)
        {
            filterCommitsBefore(argv[3]);
        }
    }
    else if ((argc == 4) && (strcmp(argv[1], "log") == 0) && (strcmp(argv[2], "-branch") == 0))
    {
        char *branch = argv[3];
        printLogContentbyBranch(branch);
    }
    else if ((argc == 4) && (strcmp(argv[1], "log") == 0) && (strcmp(argv[2], "-author") == 0))
    {
        char *author = argv[3];
        printLogContentbyAuthor(author);
    }
    else if ((argc >= 4) && (strcmp(argv[1], "log") == 0) && (strcmp(argv[2], "-search") == 0))
    {
        printLogContentbyWords(argv + 3, argc - 3);
    }
    else if ((argc == 2) && (strcmp(argv[1], "status") == 0))
    {
        return run_status();
    }
    else if ((argc == 3) && (strcmp(argv[1], "branch") == 0))
    {
        char *branch_name = argv[2];
        return run_branch(branch_name);
    }
    else if ((argc == 2) && (strcmp(argv[1], "branch") == 0))
    {
        printBranches();
    }
    /////////////////////////////////////////////////////
    else if ((argc == 8) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-m") == 0) && (strcmp(argv[6], "-c") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); 
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        int commit_id = atoi(argv[7]);
        char message[1024];
        strcpy(message, argv[5]);
        char tagname[50];
        strcpy(tagname, argv[3]);
        return add_info_to_tags_file(commit_id, message, time_str, tagname, author);
    }
    else if ((argc == 9)  && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-m") == 0) && (strcmp(argv[6], "-c") == 0) && (strcmp(argv[8], "-f") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time);
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        int commit_id = atoi(argv[7]);
        char message[1024];
        strcpy(message, argv[5]);
        char tagname[50];
        strcpy(tagname, argv[3]);
        return add_info_to_tags_file_tekrri(commit_id, message, time_str, tagname, author);
    }
    else if ((argc == 6) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-m") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time);
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        char tagname[50];
        strcpy(tagname, argv[3]);
        char message[1024];
        strcpy(message, argv[5]);
        int curr_id = extract_current_ID();
        return add_info_to_tags_file(curr_id, message, time_str, tagname, author);
    }
    else if ((argc == 7) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-m") == 0) && (strcmp(argv[6], "-f") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); 
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        char tagname[50];
        strcpy(tagname, argv[3]);
        char message[1024];
        strcpy(message, argv[5]);
        int curr_id = extract_current_ID();
        return add_info_to_tags_file_tekrri(curr_id, message, time_str, tagname, author);
    }
     else if ((argc == 6) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-c") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); 
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        char tagname[50];
        strcpy(tagname, argv[3]);
        char message[] = "";
        int commit_id = atoi(argv[5]);
        return add_info_to_tags_file(commit_id, message, time_str, tagname, author);
    }
     else if ((argc == 7) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-c") == 0) && (strcmp(argv[6], "-f") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); 
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        char tagname[50];
        strcpy(tagname, argv[3]);
        char message[] = "";
        int commit_id = atoi(argv[5]);
        return add_info_to_tags_file_tekrri(commit_id, message, time_str, tagname, author);
    }
    // neogit tag -a <tag-name> [-m <message>] [-c <commit-id>] [-f]
    else if ((argc == 4) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time);
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        char tagname[50];
        strcpy(tagname, argv[3]);
        char message[] = "";
        int curr_id = extract_current_ID();
        return add_info_to_tags_file(curr_id, message, time_str, tagname, author);
    }
    else if ((argc == 5) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-f") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time);
        char *author = (char *)malloc(50 * sizeof(char));
        author = currentAuthor();
        char tagname[50];
        strcpy(tagname, argv[3]);
        char message[] = "";
        int curr_id = extract_current_ID();
        return add_info_to_tags_file_tekrri(curr_id, message, time_str, tagname, author);
    }
    /////////////////////////////////////////////////////
    else if ((argc == 4) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "show") == 0))
    {
        char tagname[50];
        strcpy(tagname, argv[3]);
        print_target_tag(tagname);
    }
    else if ((argc == 2) && (strcmp(argv[1], "tag") == 0))
    {
        printTagNamesInOrder();
    }
   // neogit revert -n <commit-id>
    else if ((argc == 4) && (strcmp(argv[1], "revert") == 0) && (strcmp(argv[2], "-n") == 0))
    {
        return run_revert(argc, argv);
    }
    else if ((argc == 3) && (strcmp(argv[1], "revert") == 0) && (strcmp(argv[2], "-n") == 0))
    {
        return run_revert_for_last_id(argc, argv);
    }
    // neogit revert [-m <message>] HEAD-X
    //  else if ((argc == 5) && (strcmp(argv[1], "revert") == 0) && (strcmp(argv[2], "-m") == 0) && (strncmp(argv[4], "HEAD-", 5) == 0))
    // {
    //     char* endptr;
    //     long x_value = strtol(argv[4] + 5, &endptr, 10);
    //     int commit_id = extract_current_ID() - x_value;
    //      run_revertm(argc, argv);
    //     char *current_branch = (char *)malloc(50 * sizeof(char));
    //     current_branch = currentBranch();
    //     char message[1024];
    //     strcpy(message, argv[3]);
        
    //     if (strcmp(current_branch, "master") == 0)
    //     {
    //         return run_commit(argc, argv);
    //     }else{
    //         return run_commit_on_branch(argc, argv, current_branch);
    //     }
    // }
     else if ((argc == 6) && (strcmp(argv[1], "grep") == 0) && (strcmp(argv[2], "-f") == 0) && (strcmp(argv[4], "-p") == 0))
     {
        char targetWord[50] ;
        strcpy(targetWord, argv[5]);
        char filePath[PATH_MAX];
            if (realpath(argv[3], filePath) == NULL)
            {
                perror("realpath");
                exit(EXIT_FAILURE);
            }
        searchWordInFile(filePath, targetWord);
     }
     else if ((argc == 7) && (strcmp(argv[1], "grep") == 0) && (strcmp(argv[2], "-f") == 0) && (strcmp(argv[4], "-p") == 0) && (strcmp(argv[6], "-n") == 0))
     {
        char targetWord[50] ;
        strcpy(targetWord, argv[5]);
        char filePath[PATH_MAX];
            if (realpath(argv[3], filePath) == NULL)
            {
                perror("realpath");
                exit(EXIT_FAILURE);
            }
        searchWordInFileandPrintnum(filePath, targetWord);
     }
    else if ((argc == 8) && (strcmp(argv[1], "grep") == 0) && (strcmp(argv[2], "-f") == 0) && (strcmp(argv[4], "-p") == 0) && (strcmp(argv[6], "-c") == 0))
     {
        char targetWord[50] ;
        strcpy(targetWord, argv[5]);
        int commit_ID = atoi(argv[7]);
        char *fileName = strrchr(argv[3], '/');
            if (fileName != NULL)
            {
                fileName++;
                searchWordInFile_in_a_id(fileName, targetWord, commit_ID);
            } else {
                searchWordInFile_in_a_id(argv[3], targetWord, commit_ID);
            }
        
     }
    else if ((argc == 9) && (strcmp(argv[1], "grep") == 0) && (strcmp(argv[2], "-f") == 0) && (strcmp(argv[4], "-p") == 0) && (strcmp(argv[6], "-c") == 0) && (strcmp(argv[8], "-n") == 0))
     {
        char targetWord[50] ;
        strcpy(targetWord, argv[5]);
        int commit_ID = atoi(argv[7]);
        char *fileName = strrchr(argv[3], '/');
            if (fileName != NULL)
            {
                fileName++;
                searchWordInFile_in_a_id_andprintNum(fileName, targetWord, commit_ID);
            } else {
                searchWordInFile_in_a_id_andprintNum(argv[3], targetWord, commit_ID);
            }
     }
     // neogit diff -c <commit1-id> <commit2-id>
    else if((argc == 5) && (strcmp(argv[1], "diff") == 0) && (strcmp(argv[2], "-c") == 0))
    {
        int first_id = atoi(argv[3]);
        int scnd_id = atoi(argv[4]);
        find_diff_between_two(first_id, scnd_id);
    }

    // neogit diff -f <file1> <file2> –line1 <begin-end> –line2 <begin-end>
    else if ((argc == 9) && (strcmp(argv[1], "diff") == 0) && (strcmp(argv[2], "-f") == 0) && (strcmp(argv[5], "--line1") == 0) && (strcmp(argv[7], "--line2") == 0))
    {
        int f1begin, f1end, f2begin, f2end;
        sscanf(argv[6], "%d-%d", &f1begin, &f1end);
        sscanf(argv[8], "%d-%d", &f2begin, &f2end);
        f1begin--;
        f1end--;
        f2begin--;
        f2end--;
        char file1Path[PATH_MAX];
        if (realpath(argv[3], file1Path) == NULL)
        {
           perror("realpath");
           exit(EXIT_FAILURE);
        }
        char file2Path[PATH_MAX];
        if (realpath(argv[4], file2Path) == NULL)
        {
           perror("realpath");
           exit(EXIT_FAILURE);
        }
        Diff result = getDiff(file1Path, file2Path, f1begin, f1end, f2begin, f2end);
        printDifferences(&result, file1Path, file2Path);
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
    else if ((argc == 5) && (strcmp(argv[1], "diff") == 0) && (strcmp(argv[2], "-f") == 0))
    {
        int f1begin=0;
        int f2begin = 0;
        char file1Path[PATH_MAX];
        if (realpath(argv[3], file1Path) == NULL)
        {
           perror("realpath");
           exit(EXIT_FAILURE);
        }
        char file2Path[PATH_MAX];
        if (realpath(argv[4], file2Path) == NULL)
        {
           perror("realpath");
           exit(EXIT_FAILURE);
        }
        int f1end = countLines(file1Path);
        int f2end = countLines(file2Path);
        Diff result = getDiff(file1Path, file2Path, f1begin, f1end, f2begin, f2end);
        printDifferences(&result, file1Path, file2Path);
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
    // neogit merge -b <branch1> <branch2>
    else if ((argc == 5) && (strcmp(argv[1], "merge") == 0) && (strcmp(argv[2], "-b") == 0))
    {
        int lastIDofFirstBranch;
        if (strcmp(argv[3], "master") == 0){
            lastIDofFirstBranch = extract_lastID_master();
        } else {
            lastIDofFirstBranch = extract_lastID_branch(argv[3]);
        }
        int lastIDofScndBranch;
        if (strcmp(argv[4], "master") == 0){
            lastIDofScndBranch = extract_lastID_master();
        } else {
            lastIDofScndBranch = extract_lastID_branch(argv[4]);
        }
        
        char new_branchname[100];
        strcpy(new_branchname, argv[3]);
        strcat(new_branchname, "_");
        strcat(new_branchname, argv[4]);

         int merge = run_merge(lastIDofFirstBranch, lastIDofScndBranch);
         if (merge == 1){
              add_merged_branch(new_branchname);
              change_branch_in_configs(new_branchname);
              char *command = "neogit commit -m merged";
               system(command);
         }
    }

    else if ((argc == 4) && (strcmp(argv[1], "pre-commit") == 0) && (strcmp(argv[2], "hooks") == 0) && (strcmp(argv[3], "list") == 0))
    {
        printHooksList();
    }
    else if ((argc == 5) && (strcmp(argv[1], "pre-commit") == 0) && (strcmp(argv[2], "add") == 0) && (strcmp(argv[3], "hook") == 0))
    {
        addToAppliedHooks(argv[4]);
    }
    else if ((argc == 4) && (strcmp(argv[1], "pre-commit") == 0) && (strcmp(argv[2], "applied") == 0) && (strcmp(argv[3], "hooks") == 0))
    {
        print_appliedHooksList();
    }
    // neogit pre-commit remove hook <hook-id>
    else if ((argc == 5) && (strcmp(argv[1], "pre-commit") == 0) && (strcmp(argv[2], "remove") == 0) && (strcmp(argv[3], "hook") == 0))
    {
        removeAppliedHook(argv[4]);
    }
    else if ((argc == 5) && (strcmp(argv[1], "pre-commit") == 0) && (strcmp(argv[2], "-f") == 0))
    {
        char file1Path[PATH_MAX];
        if (realpath(argv[3], file1Path) == NULL)
        {
           perror("realpath");
           exit(EXIT_FAILURE);
        }
        char file2Path[PATH_MAX];
        if (realpath(argv[4], file2Path) == NULL)
        {
           perror("realpath");
           exit(EXIT_FAILURE);
        }
         run_preF(file1Path);
         run_preF(file2Path);
    }
    else if ((argc == 2) && (strcmp(argv[1], "pre-commit") == 0))
    {
        return run_pre();
    }
else if ((argc == 2))
{
    char *corr = check_and_replace_alias(argv[1]);
    if (corr != NULL) {
        system(corr);
    } else {
        printf(RED "INVALID COMMAND!\n" RESET);
    }
}

// else
// {
//     printf(RED "INVALID COMMAND!\n" RESET);
// }



    return 0;
}
