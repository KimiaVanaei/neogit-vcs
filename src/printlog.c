#include "file_functions.h"

// Define a structure to store commit information
struct CommitInfo {
    int commitID;
    char message[100];
    int numFiles;
    char operationTime[20];
};


int printLogContent()
{
    FILE *file = fopen(".neogit/commits/log", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char line[200];  
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);

    return 0;
}

int printLogContentbyNum(int n) {
    char line[100];
    struct CommitInfo commits[1000]; 
    int numCommits = 0;
    FILE *file = fopen(".neogit/commits/log", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1) {
            fgets(line, sizeof(line), file);  // Read the message line
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file);  // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file);  // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }

    fclose(file);

    int endIdx = (numCommits > n) ? n : numCommits;

    for (int i = 0; i < endIdx; i++) {
        printf("Commit ID: %d\n", commits[i].commitID);
        printf("Message: %s\n", commits[i].message);
        printf("Number of files committed: %d\n", commits[i].numFiles);
        printf("Operation done in: %s\n", commits[i].operationTime);
        printf("---------------------------------------\n");
    }

    return 0;
}

void filterCommitsSince(char *date) {
    char line[100];
    struct CommitInfo commits[1000]; 
    int numCommits = 0;
    FILE *file = fopen(".neogit/commits/log", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1) {
            fgets(line, sizeof(line), file);  // Read the message line
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file);  // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file);  // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }

    fclose(file);
    struct tm tmDate;
    strptime(date, "%Y-%m-%d %H:%M:%S", &tmDate);
    time_t targetTime = mktime(&tmDate);

    for (int i = 0; i < numCommits; ++i) {
        struct tm tmCommit;
        strptime(commits[i].operationTime, "%Y-%m-%d %H:%M:%S", &tmCommit);
        time_t commitTime = mktime(&tmCommit);

        if (commitTime >= targetTime) {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}

void filterCommitsBefore(char *date) {
    char line[100];
    struct CommitInfo commits[1000]; 
    int numCommits = 0;
    FILE *file = fopen(".neogit/commits/log", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "commit ID: %d", &commits[numCommits].commitID) == 1) {
            fgets(line, sizeof(line), file);  // Read the message line
            sscanf(line, "message: %[^\n]", commits[numCommits].message);
            fgets(line, sizeof(line), file);  // Read the numFiles line
            sscanf(line, "number of files committed: %d", &commits[numCommits].numFiles);
            fgets(line, sizeof(line), file);  // Read the operationTime line
            sscanf(line, "operation done in : %[^\n]", commits[numCommits].operationTime);

            numCommits++;
        }
    }

    fclose(file);
    struct tm tmDate;
    strptime(date, "%Y-%m-%d %H:%M:%S", &tmDate);
    time_t targetTime = mktime(&tmDate);

    for (int i = 0; i < numCommits; ++i) {
        struct tm tmCommit;
        strptime(commits[i].operationTime, "%Y-%m-%d %H:%M:%S", &tmCommit);
        time_t commitTime = mktime(&tmCommit);

        if (commitTime <= targetTime) {
            printf("Commit ID: %d\n", commits[i].commitID);
            printf("Message: %s\n", commits[i].message);
            printf("Number of files committed: %d\n", commits[i].numFiles);
            printf("Operation done in: %s\n", commits[i].operationTime);
            printf("---------------------------------------\n");
        }
    }
}
