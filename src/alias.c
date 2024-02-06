#include "file_functions.h"

int add_alias(char *newalias, char *command)
{
    char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return 1;

	char tmp_cwd[1024];
	bool exists = false;
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
				exists = true;
		}
		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return 1;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return 1;
		}

	} while (strcmp(tmp_cwd, "/") != 0);

	if (chdir(cwd) != 0)
		return 1;

	if (!exists)
	{
		perror("neogit has not been initialized");
		return 1;
	}
	else
	{
		FILE *file = fopen("/home/kimia/.neogitconfig/aliases.txt", "a");
		if (file == NULL)
			return 1;
    
    
    fprintf(file, "%s: %s\n", newalias, command);
    fclose(file);
    return 0;
    }
}
int add_localalias(char *newalias, char *command)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
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
        printf("neogit has not been initialized\n");
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
        char aliasPath[PATH_MAX];
        snprintf(aliasPath, sizeof(aliasPath), "%s/.neogit/local_aliases", currentDir);

        FILE *file = fopen(aliasPath, "a");
        if (file == NULL)
            return 1;
        fprintf(file, "%s: %s\n", newalias, command);
        fclose(file);
        if (chdir(originalCwd) != 0) {
            perror("Error changing back to original directory");
            return 1;
        }
        return 0;
    }
}

char* find_most_recent_aliasfile()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return NULL;
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
            return NULL;
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
            return NULL;
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return NULL;
        }
    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return NULL;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return NULL;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return NULL;

        char *local_aliasPath = (char *) malloc ((PATH_MAX) * sizeof(char));
        snprintf(local_aliasPath, PATH_MAX, "%s", currentDir);
        strcat(local_aliasPath, "/.neogit/local_aliases");

       
         char *glob_aliasPath = (char *) malloc ((PATH_MAX) * sizeof(char));
        strcpy(glob_aliasPath, "/home/kimia/.neogitconfig/aliases.txt");
         struct stat stat1, stat2;
        if (stat(local_aliasPath, &stat1) == -1) {
        perror("Error getting file information for file1");
        return NULL;
        }

        if (stat(glob_aliasPath, &stat2) == -1) {
        perror("Error getting file information for file2");
        return NULL;
        }
         if (stat1.st_mtime > stat2.st_mtime) {
                    return local_aliasPath;
                    
        } else if (stat1.st_mtime < stat2.st_mtime) {
            return glob_aliasPath;
        } 
    }
}
char *check_and_replace_alias_glob(char *command)
{
    char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return NULL;

	char tmp_cwd[1024];
	bool exists = false;
	struct dirent *entry;
	do
	{
		DIR *dir = opendir(".");
		if (dir == NULL)
		{
			perror("Error opening current directory");
			return NULL;
		}
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
				exists = true;
		}
		closedir(dir);

		if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
			return NULL;

		if (strcmp(tmp_cwd, "/") != 0)
		{
			if (chdir("..") != 0)
				return NULL;
		}

	} while (strcmp(tmp_cwd, "/") != 0);

	if (chdir(cwd) != 0)
		return NULL;

	if (!exists)
	{
		printf(RED "neogit has not been initialized\n" RESET);
		return NULL;
	}
	else
	{
		FILE *file = fopen("/home/kimia/.neogitconfig/aliases.txt", "r");
		if (file == NULL)
			return NULL;
            char line[1024];
        char *corr;
        char *command1;
        while (fgets(line, 1024, file) != NULL) {
       
        command1 = strtok(line, ":");
        corr = strtok(NULL, "\n");

        // Trim leading and trailing spaces from name and corr
        if (command1 != NULL) {
            while (*command1 == ' ' || *command1== '\t') {
                command1++;
            }
        }
        if (corr != NULL) {
            while (*corr == ' ' || *corr == '\t') {
                corr++;
            }
        }

        // Compare the input name with the name in the file
           if (command1 != NULL && corr != NULL && strcmp(command1, command) == 0) {
              fclose(file);
              return corr;
           }
       }
    }
}

char *check_and_replace_alias(char *command)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return NULL;
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
            return NULL;
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
            return NULL;
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return NULL;
        }
    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (!foundNeogit)
    {
        printf("neogit has not been initialized\n");
        return NULL;
    }
    else
    {
        if (chdir(tmp_cwd) != 0)
        {
            perror("Error changing directory");
            return NULL;
        }
        strcpy(currentDir, tmp_cwd);
        if (chdir(cwd) != 0)
            return NULL;
        char aliasPath[PATH_MAX];
        snprintf(aliasPath, sizeof(aliasPath), "%s/.neogit/local_aliases", currentDir);

        FILE *file = fopen("/home/kimia/.neogitconfig/aliases.txt", "r");
        if (file == NULL) {
            return NULL;
        }
        char line[1024];
        char *corr;
        char *command1;
        int found = 0;
        while (fgets(line, 1024, file) != NULL) {
       
        command1 = strtok(line, ":");
        corr = strtok(NULL, "\n");

        // Trim leading and trailing spaces from name and corr
        if (command1 != NULL) {
            while (*command1 == ' ' || *command1== '\t') {
                command1++;
            }
        }
        if (corr != NULL) {
            while (*corr == ' ' || *corr == '\t') {
                corr++;
            }
        }

        // Compare the input name with the name in the file
        if (command1 != NULL && corr != NULL && strcmp(command1, command) == 0) {
            found = 1;
              fclose(file);
              return corr;
        }
         
        }
        if (!found)
        {
                FILE *file2 = fopen(aliasPath, "r");
            if (file2 == NULL)
                return NULL;
                char line2[1024];
            char *corr2;
            char *command2;
            int found2 = 0;
                while (fgets(line2, 1024, file2) != NULL) {
            
                command2 = strtok(line2, ":");
                corr2 = strtok(NULL, "\n");

                // Trim leading and trailing spaces from name and corr
                if (command2 != NULL) {
                    while (*command2 == ' ' || *command2 == '\t') {
                        command2++;
                    }
                }
                if (corr2 != NULL) {
                    while (*corr2 == ' ' || *corr2 == '\t') {
                        corr2++;
                    }
                }

                // Compare the input name with the name in the file
                if (command2 != NULL && corr2 != NULL && strcmp(command2, command) == 0) {
                    found2 = 1;
                    fclose(file2);
                    return corr2;
                }

                } if (!found2) {
                    return NULL;
                }
       
        }
    }
}