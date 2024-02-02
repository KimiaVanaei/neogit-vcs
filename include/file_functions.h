#ifndef _FILEFUNCTIONS_H_
#define _FILEFUNCTIONS_H_
#define _GNU_SOURCE
#define PATH_MAX 4096
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
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
#include <errno.h>
#include <libgen.h>
#include <glob.h>
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
bool check_file_directory_exists_total(char *filepath);
int commit_staged_file(int commit_ID, char *filepath);
bool is_tracked(char *filepath);
int find_file_last_commit(char* filepath);
int create_log_file(int commit_ID, char *message, char *time, char *branch, char *author);
int track_file(char *filepath);
void inc_last_commit_ID();
int run_commit(int argc, char * const argv[]);
void update_username(char *new_usr);
void update_email(char *new_eml);
void update_localusername(char *new_usr);
void update_localemail(char *new_eml);
int run_checkout(int argc, char *const argv[]);
int find_file_last_change_before_commit(char *filepath, int commit_ID);
int checkout_file(char *filename, int commit_ID, char *filepath);
int remove_from_staging(char *filename);
int removeRecentStaged();
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
void create_mod_file2(const char *filename, const char *modsFilename);
int check_status2(const char *filename, const char *modsFilename);
void updateModsFile(char *fileName, time_t modTime);
void mayn();
void printFileChanges();
int printLogContent();
int printLogContentbyNum(int n);
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
void printLogContentbyWords(char **target_words, int num_words);
int saveContent(int commit_ID, char* filepath, char *filename);
int saveContent_total(int commit_ID, char *filepath, char *filename);
int run_branch(char *branch_name);
bool check_branch_directory_exists(char *filepath);
void printBranches();
void createDirectory(const char *dirPath);
void hasFileWithNumber(const char *dirPath, int targetNumber, char *branchname);
char* isInLastCommitsButNotInCurr();
long int getSavedModTime_fromstg(char *filepath);
int configs_are_set();
void change_branch_in_configs(char *new_branchname);
int checkout_file_to_branch(char *filename, char *filepath, char *branchname);
int run_checkouttobranch(char *branchname);
int containsWildcard(char *str);
void identifyWordsWithWildcard(char *pattern);
int checkout_to_id_branch(char *branchname, int commit_ID);
int checkout_file_to_id_branch(char *filename, char *filepath, char *branchname, int commit_ID);
void makeHEADzero_afterchkot_byID();
int HEADorNOT();
void makeHEADone_aftergoing_head();
int run_checkoutHEAD_forbr(char *branchname);
int run_checkoutHEAD(int argc, char *const argv[]);
int run_checkout_master(int argc, char *const argv[]);
void inc_last_commit_ID_ofbrn(char *branchname);
int saveContent_for_branch(int commit_ID, char* filepath, char *filename, char *branchname);
int run_commit_on_branch(int argc, char *const argv[], char *branchname);
int inc_last_commit_ID_total();







#endif