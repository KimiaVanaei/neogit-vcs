#include "file_functions.h"
#define MODS_FILE_NAME "mods.txt"
#define MAX_LINE_LENGTH 1024



void mayn() {
    // Open mods file in the current directory
    FILE *modsFile = fopen(MODS_FILE_NAME, "a+");
    if (modsFile == NULL) {
        perror("Error opening mods file");
        exit(EXIT_FAILURE);
    }

    // Get current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current working directory");
        fclose(modsFile);
        exit(EXIT_FAILURE);
    }

    // Open current directory
    DIR *dir = opendir(cwd);
    if (dir == NULL) {
        perror("Error opening current directory");
        fclose(modsFile);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file
        if (entry->d_type == DT_REG && strcmp(entry->d_name, MODS_FILE_NAME) != 0 && strcmp(entry->d_name, "neogit") != 0) {
            // Construct the full file path
            char filePath[PATH_MAX];

            // Attempt to resolve the path
            if (realpath(entry->d_name, filePath) == NULL) {
                perror("realpath");
                exit(EXIT_FAILURE);
            }

            // Get file information
            struct stat fileStat;
            if (stat(filePath, &fileStat) == -1) {
                perror("Error getting file information");
                continue;
            }

            // Check if the file name exists in the mods file
            char line[MAX_LINE_LENGTH];
            int found = 0;
            rewind(modsFile);
            while (fgets(line, sizeof(line), modsFile) != NULL) {
                // Extract file name and modification time from the line
                char savedFileName[MAX_LINE_LENGTH];
                time_t savedModTime;
                if (sscanf(line, "%s %ld", savedFileName, &savedModTime) == 2) {
                    // Compare with the current file name
                    if (strcmp(savedFileName, entry->d_name) == 0) {
                        found = 1;

                        // Compare modification times
                        if (savedModTime != fileStat.st_mtime) {
                            // Update modification time in the mods file
                            updateModsFile(entry->d_name, fileStat.st_mtime);
                        }
                        break;
                    }
                }
            }

            // If the file name doesn't exist in the mods file, add it
            if (!found) {
                updateModsFile(entry->d_name, fileStat.st_mtime);
            }
        }
    }

    // Close the mods file and directory
    fclose(modsFile);
    closedir(dir);


}

void updateModsFile(char *fileName, time_t modTime) {
    char currentDirectory[PATH_MAX];
    char modsFileName[4100];

    // Get the current working directory
    if (getcwd(currentDirectory, sizeof(currentDirectory)) == NULL) {
        perror("Error getting current working directory");
        exit(EXIT_FAILURE);
    }
    

    // Create the mods file name based on the current directory
    int ret = snprintf(modsFileName, sizeof(modsFileName), "%s/%s", currentDirectory, MODS_FILE_NAME);
    if (ret < 0) {
         abort();
    }

    // Open mods file for reading and writing
    FILE *modsFile = fopen(modsFileName, "r+");
    if (modsFile == NULL) {
        // If the mods file doesn't exist, create it
        modsFile = fopen(modsFileName, "w+");
        if (modsFile == NULL) {
            perror("Error opening mods file");
            exit(EXIT_FAILURE);
        }
    }

    // Search for the file in the mods file
    char line[MAX_LINE_LENGTH];
    long currentPos = 0;
    int fileFound = 0;

    while (fgets(line, sizeof(line), modsFile) != NULL) {
        char currentFileName[MAX_LINE_LENGTH];
        time_t currentModTime;

        // Parse the line to extract file name and modification time
        if (sscanf(line, "%s %ld", currentFileName, &currentModTime) == 2) {
            // Compare the current file name with the target file name
            if (strcmp(currentFileName, fileName) == 0) {
                // File found, update modification time
                fileFound = 1;
                fseek(modsFile, currentPos, SEEK_SET);
                fprintf(modsFile, "%s %ld\n", fileName, modTime);
                break;
            }
        }

        // Update the current position for seeking later
        currentPos = ftell(modsFile);
    }

    // If the file was not found, append a new entry
    if (!fileFound) {
        fprintf(modsFile, "%s %ld\n", fileName, modTime);
    }

    // Close the mods file
    fclose(modsFile);
}

void printFileChanges() {
    FILE *modsFile = fopen(MODS_FILE_NAME, "r");
    if (modsFile == NULL) {
        perror("Error opening mods file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), modsFile) != NULL) {
        char savedFileName[MAX_LINE_LENGTH];
        time_t savedModTime;

        // Parse the line to extract file name and modification time
        if (sscanf(line, "%s %ld", savedFileName, &savedModTime) == 2) {
            struct stat fileStat;

            // Attempt to resolve the path
            char filePath[PATH_MAX];
            if (realpath(savedFileName, filePath) == NULL) {
                perror("realpath");
                exit(EXIT_FAILURE);
            }

            // Check if the file exists
            if (stat(filePath, &fileStat) == -1) {
                // File not found, print deletion
                printf("%s: R\n", savedFileName);
                    mayn();
            } else {
                // Compare modification times
                if (savedModTime != fileStat.st_mtime) {
                    // Modification detected, print modification
                    printf("%s: M\n", savedFileName);
                        mayn();
                }
            }
        }
    }
    

    // Close the mods file
    fclose(modsFile);

}

