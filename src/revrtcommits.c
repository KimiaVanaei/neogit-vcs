// #include "file_functions.h"
// int run_commit_for_revert(int argc, char *const argv[], char *message)
// {
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) == NULL)
//         return 1;

//     char tmp_cwd[1024];
//     char currentDir[1024];
//     bool foundNeogit = false;
//     struct dirent *entry;

//     do
//     {
//         DIR *dir = opendir(".");
//         if (dir == NULL)
//         {
//             perror("Error opening current directory");
//             return 1;
//         }

//         while ((entry = readdir(dir)) != NULL)
//         {
//             if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
//             {
//                 foundNeogit = true;
//                 break;
//             }
//         }

//         closedir(dir);

//         if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
//             return 1;

//         if (strcmp(tmp_cwd, "/") != 0)
//         {
//             if (chdir("..") != 0)
//                 return 1;
//         }

//     } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

//     if (!foundNeogit)
//     {
//         printf("neogit has not been initialized\n");

//         return 1;
//     }
//     else
//     {
//         if (chdir(tmp_cwd) != 0)
//         {
//             perror("Error changing directory");
//             return 1;
//         }
//         strcpy(currentDir, tmp_cwd);
//         if (chdir(cwd) != 0)
//             return 1;

//         char filPath[PATH_MAX];
//         snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);
//         char allfilesPath[PATH_MAX];
//         snprintf(allfilesPath, sizeof(allfilesPath), "%s/.neogit/all_files/", currentDir);

//         if (strlen(message) > 72)
//         {
//             perror("commit message should not exceed 72 characters");
//             return 1;
//         }

//         int commit_ID = inc_last_commit_ID_total();
//         if (commit_ID == -1)
//             return 1;
//         change_last_commit_ID(commit_ID);   // for master
//         change_current_ID(commit_ID);    // total
//         char line[1024];

//         while (fgets(line, sizeof(line), file) != NULL)
//         {
//             int length = strlen(line);

//             if (length > 0 && line[length - 1] == '\n')
//             {
//                 line[length - 1] = '\0';
//             }

//             char *fileNamee = strrchr(line, '/');
//             char fileName[50];

//             if (fileNamee != NULL)
//             {
//                 fileNamee++;
//             }
//             sscanf(fileNamee, "%s", fileName);

//             if (!check_file_directory_exists(fileName))
//             {
//                 char dir_path[PATH_MAX];
//                 strcpy(dir_path, filPath);
//                 strcat(dir_path, fileName);
//                 if (mkdir(dir_path, 0755) != 0)
//                     return 1;
//             }
//             if (!check_file_directory_exists_total(fileName))
//             {
//                 char dir_path[PATH_MAX];
//                 strcpy(dir_path, allfilesPath);
//                 strcat(dir_path, fileName);
//                 if (mkdir(dir_path, 0755) != 0)
//                     return 1;
//             }
//             char line1[1024];
//             sscanf(line, "%s", line1);
//             commit_staged_file(commit_ID, line1);
//             saveContent(commit_ID, line1, fileName);
//             saveContent_total(commit_ID, line1, fileName);
//             track_file(line1);
//         }
//         fclose(file);

//         time_t t = time(NULL);
//         struct tm *current_time = localtime(&t);
//         char time_str[50];
//         strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", current_time);
//         char *branch = (char *)malloc(50 * sizeof(char));
//         char *author = (char *)malloc(50 * sizeof(char));
//         branch = currentBranch();
//         author = currentAuthor();
//         create_log_file(commit_ID, message, time_str, branch, author);

//         file = fopen(neoPath, "w");
//         if (file == NULL)
//             return 1;
//         fclose(file);

//         fprintf(stdout, "commit successfully with commit ID : %d\n", commit_ID);
//         fprintf(stdout, "operation done in : %s\n", time_str);
//         fprintf(stdout, "commit message : %s\n", message);
//         return 0;
//     }
// }