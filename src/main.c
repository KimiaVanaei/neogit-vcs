#include "file_functions.h"

int main(int argc, char *argv[])
{
    // if (argc >= 2) {
    //     if (check_and_replace_alias(argv[1]) != NULL)
    //     {

    //     }
    // }
    if ((argc < 2))
    {
        printf("invalid command\n");
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
    else if ((argc == 5) && (strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "--global") == 0) && (strncmp(argv[3], "alias", 5) == 0))
    {
        char *dotPosition = strchr(argv[3], '.');
        char *newalias = dotPosition + 1;
        add_alias(newalias, argv[4]);
    }
    else if ((argc == 4) && (strcmp(argv[1], "config") == 0) && (strncmp(argv[2], "alias", 5) == 0))
    {
        char *dotPosition = strchr(argv[2], '.');
        char *newalias = dotPosition + 1;
        add_localalias(newalias, argv[3]);
    }
    else if ((argc == 2) && (strcmp(argv[1], "add") == 0))
    {
        printf(RED "please specify a file\n" RESET);
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
    else if ((argc == 3) && (strcmp(argv[1], "checkout") == 0) && (strncmp(argv[2], "HEAD-", 4) == 0))
    {
        char* endptr;
        long n_value = strtol(argv[2] + 5, &endptr, 10);
        char *current_branch = (char *)malloc(50 * sizeof(char));
        current_branch = currentBranch();
        int last_id_of_branch = extract_lastID_branch(current_branch);
        int last_id_of_master = extract_lastID_master();
        if (strcmp(current_branch, "master") == 0)
        {
            // printf("%d\n", last_id_of_master);
            change_current_ID(last_id_of_master - n_value);
            makeHEADzero_afterchkot_byID();
        }
        else
        {
            // printf("%d\n", last_id_of_branch);
            change_current_ID(last_id_of_branch - n_value);
            makeHEADzero_afterchkot_byID();
            return checkout_to_id_branch(current_branch, last_id_of_branch - n_value);
        }

        // return run_checkout(argc, argv);
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
    else if ((argc == 3) && (strcmp(argv[1], "test") == 0))
    {
        printf("Entering 'test' block\n");
        char *str = argv[2];
        printf("String: %s\n", str);
        printf("Before containsWildcard\n");

        if (containsWildcard(str))
        {
            printf("Entering containsWildcard block\n");
            identifyWordsWithWildcard(str);
        }
        printf("Leaving 'test' block\n");
    }
    /////////////////////////////////////////////////////
    else if ((argc == 8) && (strcmp(argv[1], "tag") == 0) && (strcmp(argv[2], "-a") == 0) && (strcmp(argv[4], "-m") == 0) && (strcmp(argv[6], "-c") == 0))
    {
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        char time_str[50];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); //ejbri
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
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); //ejbri
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
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); //ejbri
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
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); //ejbri
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
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time); //ejbri
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


    return 0;
}
