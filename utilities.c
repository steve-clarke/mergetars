#include "mergetars.h"

#define TEMPLATE "/tmp/mt-XXXXXX"

char *make_temp_dir(void)
{
    char *output_directory;
    output_directory = strdup(TEMPLATE);
    mkdtemp(output_directory);

    return output_directory;
}

int fork_exec(char *path, char *args[])
{
    pid_t child = fork();
    int status = 0;

    if (child == -1) {
        perror("fork");
        status = 1;
    }
    else if (child > 0) {
        waitpid(child, &status, 0);
        status = WEXITSTATUS(status);
    }
    else {
        execv(path, args);
        exit(EXIT_FAILURE);
    }
    return status;
}

// decides which file is kept in archive based on constraints in README.md
void sort(struct stat *duplicate, FILES *original, int itemn)
{
    // latest modification time...
    if(duplicate->st_mtime > original->file_modtime) {
        original->itemn = itemn;
        original->file_size = duplicate->st_size;
        original->file_modtime = duplicate->st_mtime;
        original->file_mode = duplicate->st_mode;
    }
    // largest file size...
    else if(duplicate->st_mtime == original->file_modtime && duplicate->st_size > original->file_size) {
        original->itemn = itemn;
        original->file_size = duplicate->st_size;
        original->file_mode = duplicate->st_mode;
    }
    // last location in command line args...
    else if(duplicate->st_mtime == original->file_modtime && duplicate->st_size == original->file_size) {
        original->itemn = itemn;
        original->file_mode = duplicate->st_mode;
    }
}

// used by cleanup() to perform file deletion
extern int delete_item(const char *path, const struct stat *path_stat, int item_type, struct FTW *file_tree)
{
    switch (item_type)
    {                 // Gets the path's type
    case FTW_D:       // If a dir that has not been walked; skip it
    case FTW_DP:      // If a dir that has been walked...
        rmdir(path);  // ... remove the dir.
        break;
    default:          // If none of the above...
        unlink(path); // ...'deletes' the given file.
        break;
    }
    return 0;
}

// remove files from /tmp directory used during operation
void cleanup(int status)
{
    for (int i = 0; i < nitems; i++)
    {
        char *current_archive = items[i].tmpdir; 

        if (current_archive != NULL)
        {
            if (nftw(current_archive, delete_item, nitems, FTW_DEPTH) != 0)
            {
                perror("nftw() Error");
                exit(EXIT_FAILURE);
            }
        }
    }
    exit(status);
}

/* 
    The below function check_extension was included to help mergetars distinguish 
    which compression algorithm to use based on the user's final argument.
    However, I have since learned that GNU's tar program has featured 
    auto-compression based on filetype by default since version 1.15.
    I have included check_extension anyway, in case a future user wishes to add
    functionality. It is currently not in use in the program.
*/

bool check_extension(char *filename, char *ext)
{
    // isolate file extension
    char *dot = strpbrk(filename, ".");
    printf("%s\n", dot);
    
    // check for edge cases
    if(!dot || dot == filename) {
        return false;
    } 

    // returns true if extension to first argument matches second argument.
    return ( strcmp((dot + 1), ext ) == 0);
}
