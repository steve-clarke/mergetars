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

void cleanup(int status)
{
    exit(status);
}
