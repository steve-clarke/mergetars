#include <stdlib.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <dirent.h>   
#include <stdbool.h>  
#include <sys/wait.h>
#include <sys/param.h> 
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/types.h>

#if defined(__linux__)
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#define	TAR		"/bin/tar"
#elif   defined(__APPLE__)
#define	TAR		"/usr/bin/tar"
#endif

typedef struct {
    char	*tarfile;
    char	*tmpdir;
} ITEMS;

typedef struct {
    int         itemn;
    char        *filename;
    off_t       file_size;
    time_t      file_modtime;
    mode_t      file_mode;
} FILES;

//  defined in tars.c
extern int expand_tarfile(char *tarfile);
extern int compress_files(char *inputfiles, char *outputfile);

//  defined in utilities.c
extern char *make_temp_dir(void);
extern int fork_exec(char *path, char *args[]);
extern void sort(struct stat *statbuf, FILES *flp, int itemn);
extern void cleanup(int status);
extern bool check_extension(char *filename, char *ext);

//  defined in archives.c
extern ITEMS *add_item(char *input_tarfile);
extern ITEMS *items;
extern int nitems;

//  defined in files.c
extern void find_files(char *dirname, int itemn);
extern void add_file(int itemn, char *filename, struct stat *statbuf);
extern void populate_output(char *outputarchive);
extern void merge(void);

//  defined in directories.c
extern void add_directory(char *path);
extern void init_output_directories(char *outputtmp);

