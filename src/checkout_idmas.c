// #include "file_functions.h"

// int checkout_to_id_mas(char *branchname, int target_ID)
// {
// 	char cwd[1024];
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		return 1;
// 	char originalCwd[1024];
// 	strcpy(originalCwd, cwd);

// 	char tmp_cwd[1024];
// 	char currentDir[1024];
// 	bool foundNeogit = false;
// 	struct dirent *entry;

// 	do
// 	{
// 		DIR *dir = opendir(".");
// 		if (dir == NULL)
// 		{
// 			perror("Error opening current directory");
// 			return 1;
// 		}

// 		while ((entry = readdir(dir)) != NULL)
// 		{
// 			if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
// 			{
// 				foundNeogit = true;
// 				break;
// 			}
// 		}

// 		closedir(dir);

// 		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
// 			return 1;

// 		if (strcmp(tmp_cwd, "/") != 0)
// 		{
// 			if (chdir("..") != 0)
// 				return 1;
// 		}

// 	} while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

// 	if (!foundNeogit)
// 	{
// 		printf("neogit has not been initialized\n");

// 		return 1;
// 	}
// 	else
// 	{
// 		if (chdir(tmp_cwd) != 0)
// 		{
// 			perror("Error changing directory");
// 			return 1;
// 		}
// 		strcpy(currentDir, tmp_cwd);
// 		if (chdir(cwd) != 0)
// 			return 1;

// 		char neoPath[PATH_MAX];
// 		snprintf(neoPath, sizeof(neoPath), "%s/.neogit/staging", currentDir);
// 		FILE *stgFile = fopen(neoPath, "r");
// 		if (stgFile == NULL) return 1;
// 		fseek(stgFile, 0, SEEK_END);
// 		long size = ftell(stgFile);	 
// 		rewind(stgFile);
// 		if (size != 0)
// 		{
// 			printf("You have uncommitted files. You can`t checkout\n");
// 			return 1;
// 		}
// 		if (chdir(originalCwd) != 0)
// 		{
// 			perror("Error restoring original directory");
// 			return 1;
// 		}

// 		DIR *dir = opendir(".");
// 		struct dirent *entry;
// 		while ((entry = readdir(dir)) != NULL)
// 		{
// 			if (entry->d_type == DT_REG)
// 			{
// 				char filePath[PATH_MAX];
// 				if (realpath(entry->d_name, filePath) == NULL)
// 				{
// 					perror("realpath");
// 					exit(EXIT_FAILURE);
// 				}

// 				if (is_tracked(filePath))
//                 checkout_file_to_id_mas(entry->d_name, filePath, target_ID);
// 			}
// 		}
// 		closedir(dir);
//         makeHEADzero_afterchkot_byID();
// 		return 0;
// 	}
// }

// int checkout_file_to_id_mas(char *filename, char *filepath, int target_ID)
// {
//     char cwd[1024];
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		return 1;

// 	char tmp_cwd[1024];
// 	char currentDir[1024];
// 	bool foundNeogit = false;
// 	struct dirent *entry;

// 	do
// 	{
// 		DIR *dir = opendir(".");
// 		if (dir == NULL)
// 		{
// 			perror("Error opening current directory");
// 			return 1;
// 		}

// 		while ((entry = readdir(dir)) != NULL)
// 		{
// 			if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
// 			{
// 				foundNeogit = true;
// 				break;
// 			}
// 		}

// 		closedir(dir);

// 		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
// 			return 1;

// 		if (strcmp(tmp_cwd, "/") != 0)
// 		{
// 			if (chdir("..") != 0)
// 				return 1;
// 		}

// 	} while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

// 	if (foundNeogit)
// 	{
// 		if (chdir(tmp_cwd) != 0)
// 		{
// 			perror("Error changing directory");
// 			return 1;
// 		}
// 		strcpy(currentDir, tmp_cwd);
// 		if (chdir(cwd) != 0)
// 			return 1;
	
// 	char filPath[PATH_MAX];
// 	char target_path[PATH_MAX];
// 	snprintf(filPath, sizeof(filPath), "%s/.neogit/files/", currentDir);
//     strcat(filPath, branchname);
//     strcat(filPath, "/");
// 	// printf("%s\n", filPath);       // /mnt/c/c_test/proj/.neogit/branches/bery/                                                         
// 	char branchDir[PATH_MAX];
// 	strcpy(branchDir, filPath);
// 	DIR *dir2 = opendir(branchDir);
	
// 	int count = 0;
// 	struct dirent* entry2;
// 	while ((entry2 = readdir(dir2)) != NULL) {
//         if (entry2->d_type == DT_DIR && strcmp(entry2->d_name , ".") != 0 && strcmp(entry2->d_name , "..") != 0) {
//             count++;
//             if (count == target_ID) {
// 				strcpy(target_path, branchDir);
// 				strcat(target_path, entry2->d_name);
//                 strcat(target_path, "/");
//                 break;
//             }
//         }
//     }
//     closedir(dir2);
// 	// printf("%s\n", target_path);          // /mnt/c/c_test/proj/.neogit/branches/hihi/2/
//     char src_file[1024];
//     strcpy(src_file, target_path);
//     strcat(src_file, filename);
// 	FILE *read_file = fopen(src_file, "r");
// 	if (read_file == NULL) return 1;
// 	FILE *write_file = fopen(filepath, "w");
// 	if (write_file == NULL) return 1;
// 	char line2[1024];
// 	while (fgets(line2, sizeof(line2), read_file) != NULL)
// 	{
// 		fprintf(write_file, "%s", line2);
// 	}
// 	fclose(read_file);
// 	fclose(write_file);
// 	return 0;
//     }
// }