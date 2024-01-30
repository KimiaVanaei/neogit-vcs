#include "file_functions.h"
void create_mod_file2(const char *filename, const char *modsFilename)
{
     char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return;

    char tmp_cwd[1024];
    char currentDir[1024];
    bool foundNeogit = false;
    struct dirent *entry;

    do {
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0) {
                foundNeogit = true;
                break;
            }
        }

        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return;

        printf("%s\n", tmp_cwd);

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0)
                return;
        }

    } while (!foundNeogit && strcmp(tmp_cwd, "/") != 0);

    if (foundNeogit) {
        // Change the current working directory to the one containing .neogit
        if (chdir(tmp_cwd) != 0) {
            perror("Error changing directory");
            return;
        }
        strcpy(currentDir, tmp_cwd);


    } else {
        printf("neogit has not been initialized\n");
    }

  
    
    char fullModsPath[PATH_MAX];
    snprintf(fullModsPath, sizeof(fullModsPath), "%s/.neogit/%s", currentDir, modsFilename);
    // printf("current path : %s\n", fullModsPath);
    if (access(fullModsPath, F_OK) != -1)
    {
        printf("%s: Mod file already exists. Skipping.\n", filename);
        return;
    }
    char resolved_path[PATH_MAX];
    char *result = realpath(filename, resolved_path);

    if (result == NULL) {
        perror("realpath");
        return;
    }

    struct stat fileStat;
    if (stat(resolved_path, &fileStat) == -1)
    {
        perror("Error getting file status");
        return;
    }
        FILE *modsFile = fopen(fullModsPath, "w");
        if (modsFile == NULL)
        {
            perror("Error opening mods file");
            return;
        }
        fprintf(modsFile, "%ld", fileStat.st_mtime);
        fclose(modsFile);

    
}

int check_status2(const char *filename, const char *modsFilename)
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

        printf("%s\n", tmp_cwd);

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


    } else {
        printf("neogit has not been initialized\n");
    }

  
    
    char fullModsPath[PATH_MAX];
    snprintf(fullModsPath, sizeof(fullModsPath), "%s/.neogit/%s", currentDir, modsFilename);
    char resolved_path[PATH_MAX];
    char *result = realpath(filename, resolved_path);

    if (result == NULL) {
        perror("realpath");
        return 1;
    }
    struct stat fileStat;
    if (stat(resolved_path, &fileStat) == -1)
    {
        perror("Error getting file status");
        return 1;
    }

    FILE *modsFile = fopen(fullModsPath, "r");
    if (modsFile == NULL)
    {
        perror("Error checking mod file");
        return 1;
    }
    
        time_t storedModificationTime;
        if (fscanf(modsFile, "%ld", &storedModificationTime) != 1)
        {
            perror("Error reading stored modification time");
            fclose(modsFile);
            return 1;
        }

        fclose(modsFile);

        // Compare the stored modification time with the current modification time
        if (storedModificationTime == fileStat.st_mtime)
        {
            printf("%s: File has not been modified.\n", filename);
        }
        else
        {
            printf("%s: File has been modified.\n", filename);

            // Display the current modification time of the file
            printf("%s: Current Modification Time: %ld\n", filename, fileStat.st_mtime);

            // Update the stored modification time in the mods file
            modsFile = fopen(fullModsPath, "w");
            if (modsFile == NULL)
            {
                perror("Error opening mods file");
                return 1;
            }

            fprintf(modsFile, "%ld", fileStat.st_mtime);
            fclose(modsFile);
        }
    

    return 0;
}

int primer()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
         perror("getcwd");
        return EXIT_FAILURE;
    }
    DIR *dir = opendir(cwd);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        struct stat fileStat;
        if (stat(entry->d_name, &fileStat) == -1)
         {
           perror("Error getting file status");
           return 1;
         }

    if (S_ISREG(fileStat.st_mode))
    {
        char modsFilename[PATH_MAX];
        strncpy(modsFilename, entry->d_name, sizeof(modsFilename));
        strcat(modsFilename, "mod");
        // Create the mod file for each file
    //    create_mod_file(entry->d_name,modsFilename);
    //    check_status(entry->d_name, modsFilename);
    }

    
    }

    closedir(dir);

    return 0;
}
