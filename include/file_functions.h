#ifndef _FILEFUNCTIONS_H_
#define _FILEFUNCTIONS_H_
#define _GNU_SOURCE
#define PATH_MAX 4096
#define STR_LINE_MAX 1024
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define BRMAG   "\x1B[95m"
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
struct tagInfo {
    char tagname[50];
    int commitID;
    char author[50];
    char message[100];
    char operationTime[20];
};
typedef struct {
    char **deletedLines;
    unsigned int *lineNumberDeleted;
    size_t deletedCount;

    char **addedLines;
    unsigned int *lineNumberAdded;
    size_t addedCount;
} Diff;
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
bool check_mas_directory_exists();
int commit_staged_file(int commit_ID, char *filepath, time_t modification_time);
bool is_tracked(char *filepath);
int find_file_last_commit(char* filepath);
int create_log_file(int commit_ID, char *message, char *time, char *branch, char *author);
int track_file(char *filepath);
void change_last_commit_ID(int new_id);
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
int run_redo(char *filepath);

int add_localalias(char *newalias, char *command);
int add_alias(char *newalias, char *command);
char* check_and_replace_alias(char *command, char *alias_filePath);
char* find_most_recent_aliasfile();

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
int matchesWildcard(const char *str, const char *pattern);

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
void identifyWordsWithWildcard(char *pattern);;
int checkout_to_id_branch(char *branchname, int commit_ID);
int checkout_file_to_id_branch(char *filename, char *filepath, char *branchname, int commit_ID);
void makeHEADzero_afterchkot_byID();
int HEADorNOT();
void makeHEADone_aftergoing_head();
int run_checkoutHEAD_forbr(char *branchname);
int run_checkoutHEAD(int argc, char *const argv[]);
int run_checkout_master(int argc, char *const argv[]);
void change_last_commit_ID_ofbrn(char *branchname, int new_id);
int saveContent_for_branch(int commit_ID, char* filepath, char *filename, char *branchname);
int run_commit_on_branch(int argc, char *const argv[], char *branchname);
int inc_last_commit_ID_total();
int extract_current_ID();
int extract_lastID_master();
int extract_lastID_branch(char *branchname);
int extract_lastID_total();
void change_current_ID(int new_currentID);

int add_info_to_tags_file(int commit_ID, char *message, char *time, char *tagname, char *author);
void print_target_tag(char *tagname);
bool check_tag_exists(char *tagname);
void overwrite_existing_tag(int commit_ID, char *message, char *time, char *tagname, char *author);
int add_info_to_tags_file_tekrri(int commit_ID, char *message, char *time, char *tagname, char *author);


void swap(struct tagInfo *a, struct tagInfo *b);
void printTagNamesInOrder();

int run_revert(int argc, char *const argv[]);
int run_revert_for_last_id(int argc, char *const argv[]);
int checkout_file_to_id_branch2(char *filename, char *filepath, char *branchname, int N);
int run_revertm_HEAD(char *branchname, int N);

void searchWordInFile(char *filePath, char *searchWord) ;
int isWordMatch(const char *line, const char *searchWord);
void searchWordInFileandPrintnum(char *filePath, char *searchWord);
void searchWordInFile_in_a_id(char *filename, char *targetWord,int commit_ID);
void searchWordInFile_in_a_id_andprintNum(char *filename, char *targetWord,int commit_ID);

void find_diff_between_two(int first_id, int second_id);
void run_diff_for_matched(char *commonPath, int first_id, int scnd_id);
void run_diff_for_matched2(char *commonPath, int first_id, int scnd_id);
char *strTrim(char *str);
char *strDuplicate(const char *str);
int compareLines(FILE *baseFile, FILE *changedFile, char *line1, char *line2, unsigned int *line1_cnt, unsigned int *line2_cnt);
Diff getDiff(const char *baseFilePath, const char *changedFilePath, int f1begin, int f1end, int f2begin, int f2end);
void printDifferences(const Diff *diff, const char *baseFilePath, const char *changedFilePath);
void normalizeLineEndings(char *line);
int countLines(const char *filePath);

int add_merged_branch(char *branch_name);
int run_merge(int first_id, int second_id);
void printDifferences_merge(const Diff *diff, const char *baseFilePath, const char *changedFilePath, const char *filename);






#endif