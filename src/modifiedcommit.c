#include "file_functions.h"

#define MAX_PATH_LENGTH 1024
#define MAX_FILE_CONTENT 2000


// Function to create a new commit
Commit* createCommit(const char* message, Commit* parent) {
    Commit* newCommit = (Commit*)malloc(sizeof(Commit));
    strcpy(newCommit->message, message);
    newCommit->parent = parent;
    newCommit->numFiles = 0;
    return newCommit;
}

// Function to create a new branch
Branch* createBranch(const char* name) {
    Branch* newBranch = (Branch*)malloc(sizeof(Branch));
    strcpy(newBranch->name, name);
    newBranch->head = NULL;
    return newBranch;
}

// Function to switch to a branch
void switchBranch(Branch* branch) {
    if (branch->head == NULL) {
        printf("Error: Branch '%s' has no commits.\n", branch->name);
        return;
    }

    printf("Switched to branch '%s'\n", branch->name);
}

// Function to commit changes to the current branch
void commit(Branch* branch, const char* message) {
    Commit* newCommit = createCommit(message, branch->head);
    branch->head = newCommit;
    printf("Committed: %s\n", message);
}

// Function to add an imaginary file to the current commit
void addFile(Commit* commit, const char* filename) {
    if (commit->numFiles < 10) {
        strcpy(commit->files[commit->numFiles], filename);
        commit->numFiles++;
    } else {
        printf("Error: Maximum number of files reached for a commit.\n");
    }
}

// Function to read the content of an imaginary file
void readFileContent(const char* filename, char* content) {
    char filepath[MAX_PATH_LENGTH];
    sprintf(filepath, "%s", filename);

    FILE* file = fopen(filepath, "r");
    if (file) {
        fgets(content, MAX_FILE_CONTENT, file);
        fclose(file);
    } else {
        printf("Error: File '%s' not found.\n", filename);
    }
}

// Function to write content to an imaginary file
void writeFileContent(const char* filename, const char* content) {
    char filepath[MAX_PATH_LENGTH];
    sprintf(filepath, "%s", filename);

    FILE* file = fopen(filepath, "w");
    if (file) {
        fprintf(file, "%s", content);
        fclose(file);
    } else {
        printf("Error: Unable to write to file '%s'.\n", filename);
    }
}

// Function to display commit history of the current branch
void showHistory(Branch* branch) {
    printf("Commit history for branch '%s':\n", branch->name);
    Commit* currentCommit = branch->head;

    while (currentCommit != NULL) {
        printf("Commit: %s\n", currentCommit->message);

        for (int i = 0; i < currentCommit->numFiles; i++) {
            char content[MAX_FILE_CONTENT];
            readFileContent(currentCommit->files[i], content);
            printf("File '%s' content: %s\n", currentCommit->files[i], content);
        }

        printf("\n");
        currentCommit = currentCommit->parent;
    }
}

// Function to initialize a new repository
void initializeRepository() {

    // Create HEAD file
    FILE* headFile = fopen(".neogit/HEAD", "w");
    fprintf(headFile, "master\n");
    fclose(headFile);

    printf("Initialized empty repository.\n");
}

int man() {
    // Initialize the repository
    initializeRepository();

    // Open and read the HEAD file to determine the current branch
    char currentBranch[MAX_PATH_LENGTH];
    FILE* headFile = fopen(".neogit/HEAD", "r");
    fscanf(headFile, "%s", currentBranch);
    fclose(headFile);

    // Create the master branch
    Branch* master = createBranch("master");

    // Perform some commits on the master branch
    commit(master, "Initial commit");
    addFile(master->head, "hi.txt");
    writeFileContent("hi.txt", "Content of file1");
    commit(master, "Fix bug");
    addFile(master->head, "mods.txt");
    writeFileContent("mods.txt", "Content of file2");
    commit(master, "Add feature");

    // Display commit history of the master branch
    showHistory(master);

    // Switch to a new branch
    switchBranch(master);

    // Create and switch to a feature branch
    Branch* featureBranch = createBranch("feature_branch");
    switchBranch(featureBranch);

    // Perform some commits on the feature branch
    commit(featureBranch, "Implement new feature");
    addFile(featureBranch->head, "file3.txt");
    writeFileContent("file3.txt", "Content of file3");
    commit(featureBranch, "Update feature");

    // Display commit history of the feature branch
    showHistory(featureBranch);

    // Switch back to the master branch
    switchBranch(master);

    // Display updated commit history of the master branch
    showHistory(master);

    // Free memory
    free(master);
    free(featureBranch);

    return 0;
}
