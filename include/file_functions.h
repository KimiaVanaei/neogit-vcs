#ifndef _FILEFUNCTIONS_H_
#define _FILEFUNCTIONS_H_
#define _GNU_SOURCE
#define PATH_MAX 4096
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
typedef struct Commit {
    char message[100];
    struct Commit* parent;
    char files[10][50];  // List of imaginary files in this commit
    int numFiles;
} Commit;

// Structure to represent a branch
typedef struct Branch {
    char name[50];
    Commit* head;
} Branch;
int run_add(int argc, char *const argv[]);
int create_configs(char *username, char *email);
int run_init();
int add_to_staging(char *filepath);
int add_to_staging_recursive(char *dirpath);
bool check_file_directory_exists(char *filepath);
int commit_staged_file(int commit_ID, char* filepath);
bool is_tracked(char *filepath);
int find_file_last_commit(char* filepath);
int create_log_file(int commit_ID, char *message, char *time, char *branch, char *author);
int track_file(char *filepath);
int inc_last_commit_ID();
int run_commit(int argc, char * const argv[]);
void update_username(char *new_usr);
void update_email(char *new_eml);
void update_localusername(char *new_usr);
void update_localemail(char *new_eml);
int run_checkout(int argc, char *const argv[]);
int find_file_last_change_before_commit(char *filepath, int commit_ID);
int checkout_file(char *filepath, int commit_ID);
int remove_from_staging(char *filename);
int run_add_single(char *path);
int add_depth(const char *currentDir, int depth);
int isStaged(const char *filename);
int add_localalias(char *newalias, char *command);
int add_alias(char *newalias, char *command);
char* check_and_replace_alias(char *command);
char* get_alias_from_file(char *filename, char *command);
int add_shrtcut_for_msg(char *newshortcut, char *msg);
void update_msg(char *new_msg, char *shrtcut);
void delete_msg(char *shrtcut);
// void checkStaging(const char *currentDir, int depth);
// void checkStagingRecursive(const char *currentDir, int depth);
// int check_status(const char *filename, const char *modsFilename);
// int primer();
// int create_mod_file(const char *filename, const char *modsFilename);
// int isDirectoryExists(const char *path);
void create_mod_file2(const char *filename, const char *modsFilename);
int check_status2(const char *filename, const char *modsFilename);
void updateModsFile(char *fileName, time_t modTime);
void mayn();
void printFileChanges();
Commit* createCommit(const char* message, Commit* parent);
Branch* createBranch(const char* name);
void switchBranch(Branch* branch);
void commit(Branch* branch, const char* message);
void addFile(Commit* commit, const char* filename);
void readFileContent(const char* filename, char* content);
void writeFileContent(const char* filename, const char* content);
void showHistory(Branch* branch);
void initializeRepository();
int man();
int printLogContent();
int printLogContentbyNum(int n);
int printlastline();
void filterCommitsSince(char *date);
void filterCommitsBefore(char *date);
int run_status();
int isInLastCommits(char *filepath, int last_ID);
long int getSavedModTime(char *filepath, int last_ID);
int last_ID();
char *currentAuthor();
char *currentBranch();
void printLogContentbyBranch(char *target_branch);
void printLogContentbyAuthor(char *target_author);
void printLogContentbyWord(char *target_word);








#endif