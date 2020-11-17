
#include <stdlib.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/param.h>  
#include <dirent.h>     
#include <sys/stat.h>
#include <sys/time.h>

#if defined(__linux__)
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define	TAR		"/bin/tar"
#elif   defined(__APPLE__)
#define	TAR		"/usr/bin/tar"
#endif

#define MAX_FILES       100

#define MAX_TARS        100

#define TEMPLATE        "/tmp/mt-XXXXXX"

#define TEMP_PATH_LEN   15

#define BUF_SIZE        4096

// ----------------------- ARCHIVE STRUCTURE ----------------------------------

struct Archive // Change struct name to all caps? (Lecture from Chris says its standard style)
{
    char *tmp_folder;
    int num_files;
    int num_dirs;
    struct File
    {
        char *pathname;
    } files[MAX_FILES];
    struct Directory
    {
        char *dir_name;
    } directories[MAX_FILES]; 
} archives[MAX_TARS]; 

// ----------------------- ITEM STRUCTURE -------------------------------------

struct Item 
{
    char *file_name;   // Not including /tmp/mnt-ddddd/
    int num_instances; // Size of the Instance array below
    struct Instance
    {
        char *full_path;
        time_t mod_time;
        int size;
    } instances[MAX_TARS];
} items[MAX_FILES];

// ---------------------- GLOBAL VARIABLES ------------------------------------

// Number of input .tars specified by user
int input_file_count;
// Name of merged .tar file specified by user
char *output_archive_name; 

// Global variables needed for find_files recursion
int file_counter; 
int dir_counter;

// --------------------------- DIR FUNCTIONS ----------------------------------

extern char *make_temp_dir(void);
extern void archive_output(char *directory);
extern char *expand_tar(char *filename);
extern int delete_item(const char *path, const struct stat *path_stat, int item_type, struct FTW *file_tree);
extern void clean_temps(void);
extern void init_output_directories(void);
extern void expand_all(char *argv[], int input_file_count);
extern int check_extension(char *filename);

// ---------------------------- SORT FUNCTIONS --------------------------------

extern void sort_files(void);
extern int latest_tar(struct Instance x, int x_index, struct Instance y, int y_index);
extern int biggest_file(struct Instance x, int x_index, struct Instance y, int y_index);
extern int newest_file(struct Instance x, int x_index, struct Instance y, int y_index);

// ---------------------------- PRINT FUNCTIONS --------------------------------

extern void print_input_data(char *argv[], int input_count);
extern void print_data_structure(void);
extern void print_outputs(void);
extern void print_items_array(void);

// ----------------------- DATA STRUCT POPULATORS -------------------------------

extern void populate_output(void);
extern void find_files(char *dir_name, int tar_index);
extern void add_instances(char *file_name);
extern void scan_archives(void);
